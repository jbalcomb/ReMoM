#!/usr/bin/env python
"""
vet_dependency.py - gather the checkable facts about a third-party dependency.

This tool deliberately does NOT produce a trust score.  A number invites you to stop thinking, and
the criteria behind it are never as rigorous as the number implies.  What it does instead is fetch
the facts that are actually verifiable, flag the ones that historically precede supply-chain
compromise, and print the questions it CANNOT answer so they are recorded as gaps rather than
silently omitted.

The policy this implements - what the tiers mean, why each check exists, and the xz-utils worked
example - lives in doc/#Devel/Dependency-Vetting.md.  Read that first.

Usage:
  python tools/vet_dependency.py --repo OWNER/NAME
                                 [--upstream OWNER/NAME --author "Author Name"]
                                 [--manifest-url URL]
                                 [--tier A|B|C]
                                 [--json]

  --repo          The repo that publishes the artifact you would install.
  --upstream      The project the artifact is built from, if different (e.g. FFmpeg/FFmpeg for a
                  third-party FFmpeg build).  Enables the tenure checks.
  --author        Author name as it appears in upstream commits, for the tenure lookup.
  --manifest-url  A raw package-manifest URL (winget/homebrew/etc.) to check for hash pinning.
  --tier          Blast-radius tier, per the policy doc.  Only affects the printed reminder.

Example - the case this tool was written for:
  python tools/vet_dependency.py --repo GyanD/codexffmpeg \
      --upstream FFmpeg/FFmpeg --author "Gyan Doshi" --tier C

GitHub's unauthenticated API allows 60 requests/hour (10/min for search).  Set GITHUB_TOKEN in the
environment to raise that; the script works without one but may hit 403s.
"""

from __future__ import print_function

import argparse
import datetime
import json
import os
import sys

try:
    from urllib.request import urlopen, Request
    from urllib.error import HTTPError, URLError
except ImportError:  # Python 2
    from urllib2 import urlopen, Request, HTTPError, URLError


GITHUB_API = "https://api.github.com"

# Thresholds below which a signal is worth a human's attention.  These are judgement calls, not
# science; the policy doc explains the reasoning behind each.
MIN_UPSTREAM_TENURE_DAYS = 730       # 2 years of contribution before you are "established"
MIN_AUTHORITY_LAG_DAYS = 365         # contribution history before taking on publishing authority
MIN_PUBLISHER_AGE_DAYS = 365         # how long the distribution channel has existed
MAX_INACTIVITY_DAYS = 365            # abandoned projects do not get security fixes


def http_get_json(url):
    """GET a JSON endpoint. Returns (data, error_string)."""
    headers = {
        "Accept": "application/vnd.github+json",
        "User-Agent": "ReMoM-vet-dependency",
    }
    token = os.environ.get("GITHUB_TOKEN")
    if token:
        headers["Authorization"] = "Bearer " + token

    try:
        request = Request(url, headers=headers)
        response = urlopen(request, timeout=30)
        payload = response.read()
        if not isinstance(payload, str):
            payload = payload.decode("utf-8", "replace")
        return json.loads(payload), None
    except HTTPError as exc:
        detail = ""
        if exc.code == 403:
            detail = " (rate limited? set GITHUB_TOKEN)"
        return None, "HTTP %s%s" % (exc.code, detail)
    except URLError as exc:
        return None, "network error: %s" % (exc.reason,)
    except ValueError as exc:
        return None, "bad JSON: %s" % (exc,)


def http_get_text(url):
    """GET a plain-text URL. Returns (text, error_string)."""
    try:
        request = Request(url, headers={"User-Agent": "ReMoM-vet-dependency"})
        response = urlopen(request, timeout=30)
        payload = response.read()
        if not isinstance(payload, str):
            payload = payload.decode("utf-8", "replace")
        return payload, None
    except HTTPError as exc:
        return None, "HTTP %s" % (exc.code,)
    except URLError as exc:
        return None, "network error: %s" % (exc.reason,)


def parse_iso(text):
    if not text:
        return None
    try:
        return datetime.datetime.strptime(text[:19], "%Y-%m-%dT%H:%M:%S")
    except ValueError:
        return None


def days_between(earlier, later):
    if earlier is None or later is None:
        return None
    return (later - earlier).days


def years(days):
    if days is None:
        return "?"
    return "%.1f yr" % (days / 365.25,)


def probe_publisher(repo, findings):
    """The repo that publishes the artifact you would actually install."""
    data, error = http_get_json("%s/repos/%s" % (GITHUB_API, repo))
    if error:
        findings["errors"].append("publisher repo %s: %s" % (repo, error))
        return

    now = datetime.datetime.utcnow()
    created = parse_iso(data.get("created_at"))
    pushed = parse_iso(data.get("pushed_at"))

    findings["publisher"] = {
        "repo": repo,
        "created_at": data.get("created_at"),
        "pushed_at": data.get("pushed_at"),
        "age_days": days_between(created, now),
        "days_since_push": days_between(pushed, now),
        "is_fork": data.get("fork"),
        "archived": data.get("archived"),
        "stars": data.get("stargazers_count"),
        "license": (data.get("license") or {}).get("spdx_id"),
    }


def probe_releases(repo, findings):
    data, error = http_get_json("%s/repos/%s/releases?per_page=100" % (GITHUB_API, repo))
    if error:
        findings["errors"].append("releases %s: %s" % (repo, error))
        return
    if not isinstance(data, list) or not data:
        findings["releases"] = {"count": 0}
        return

    dates = sorted([d for d in (parse_iso(r.get("published_at")) for r in data) if d])
    findings["releases"] = {
        "count_recent_page": len(data),
        "oldest_on_page": dates[0].isoformat() if dates else None,
        "newest_on_page": dates[-1].isoformat() if dates else None,
    }


def probe_tenure(upstream, author, findings):
    """
    How long has this person been contributing to the UPSTREAM project, and did that history predate
    them taking on publishing authority?

    This is the check that "is listed in MAINTAINERS" completely misses.  A maintainer entry with no
    date attached is nearly content-free - what matters is tenure, and tenure relative to how long
    the artifact has existed.  See the xz-utils example in the policy doc.
    """
    # The author name MUST be quoted.  Unquoted, `author-name:Gyan Doshi` degrades to
    # `author-name:Gyan` AND a loose term "Doshi", which silently returns a different (wrong) count
    # than an exact-name match.  Getting this wrong produces a confident number that means nothing --
    # exactly the failure mode this tool exists to prevent.
    query = "repo:%s+author-name:%%22%s%%22" % (upstream, author.replace(" ", "+"))

    oldest, error = http_get_json(
        "%s/search/commits?q=%s&sort=author-date&order=asc&per_page=1" % (GITHUB_API, query))
    if error:
        findings["errors"].append("tenure search (%s / %s): %s" % (upstream, author, error))
        return

    items = oldest.get("items") or []
    if not items:
        findings["errors"].append(
            "tenure search (%s / %s): no commits matched - check the author name spelling"
            % (upstream, author))
        return

    first_commit = parse_iso((items[0].get("commit", {}).get("author") or {}).get("date"))
    now = datetime.datetime.utcnow()

    publisher_created = None
    if findings.get("publisher"):
        publisher_created = parse_iso(findings["publisher"].get("created_at"))

    findings["tenure"] = {
        "upstream": upstream,
        "author": author,
        "total_commits": oldest.get("total_count"),
        "first_commit": first_commit.isoformat() if first_commit else None,
        "tenure_days": days_between(first_commit, now),
        # Time between "started contributing upstream" and "started publishing artifacts".
        # A large positive number is reassuring: they earned standing before taking authority.
        "authority_lag_days": days_between(first_commit, publisher_created),
    }


def probe_manifest(url, findings):
    """Does the package manifest pin a hash, and where does it actually point?"""
    text, error = http_get_text(url)
    if error:
        findings["errors"].append("manifest %s: %s" % (url, error))
        return

    lowered = text.lower()
    urls = []
    for line in text.splitlines():
        stripped = line.strip()
        if "http" in stripped and ("url" in stripped.lower() or stripped.startswith("- ")):
            urls.append(stripped[:200])

    findings["manifest"] = {
        "url": url,
        "pins_sha256": ("sha256" in lowered),
        "pins_sha512": ("sha512" in lowered),
        "referenced_urls": urls[:10],
    }


def evaluate_flags(findings):
    """Turn the gathered facts into attention-worthy flags. Never into a score."""
    flags = []
    good = []

    publisher = findings.get("publisher")
    if publisher:
        age = publisher.get("age_days")
        if age is not None:
            if age < MIN_PUBLISHER_AGE_DAYS:
                flags.append("Distribution repo is only %s old - young publishing channel." % years(age))
            else:
                good.append("Distribution repo has existed %s." % years(age))
        idle = publisher.get("days_since_push")
        if idle is not None:
            if idle > MAX_INACTIVITY_DAYS:
                flags.append("No push in %d days - possibly abandoned; abandoned deps do not get security fixes." % idle)
            else:
                good.append("Active: last push %d day(s) ago." % idle)
        if publisher.get("archived"):
            flags.append("Repo is ARCHIVED.")

    tenure = findings.get("tenure")
    if tenure:
        tenure_days = tenure.get("tenure_days")
        if tenure_days is not None:
            if tenure_days < MIN_UPSTREAM_TENURE_DAYS:
                flags.append("Upstream tenure is only %s - short history relative to the trust being extended." % years(tenure_days))
            else:
                good.append("Upstream tenure %s (%s commits)." % (years(tenure_days), tenure.get("total_commits")))

        lag = tenure.get("authority_lag_days")
        if lag is not None:
            if lag < 0:
                flags.append("Began publishing artifacts BEFORE their first upstream commit - inverted trust order.")
            elif lag < MIN_AUTHORITY_LAG_DAYS:
                flags.append("Only %s between first upstream commit and taking on publishing authority - fast escalation (this is the xz-utils shape)." % years(lag))
            else:
                good.append("Contributed upstream for %s before taking on publishing authority." % years(lag))

    manifest = findings.get("manifest")
    if manifest:
        if manifest.get("pins_sha256") or manifest.get("pins_sha512"):
            good.append("Package manifest pins a content hash - a swapped binary would be detected.")
        else:
            flags.append("Package manifest does NOT appear to pin a content hash.")

    return flags, good


# Questions this tool structurally cannot answer.  Printed every run so they are recorded as known
# gaps rather than quietly omitted - which was the original failure this tool exists to prevent.
UNANSWERABLE = [
    "Is the shipped binary reproducible from the public source? (The xz backdoor was in the release "
    "tarball but NOT the git tree - tenure would never have caught it; only build transparency would.)",
    "Has the maintainer's account or signing key been compromised? Tenure defends against "
    "infiltration, not against a stolen key today.",
    "Sanctions / entity-list screening of the publishing organisation, where legally required "
    "(NIST SP 800-161, Section 889, OFAC SDN, BIS Entity List). Not automatable here, and for an "
    "individual contributor, nationality inferred from a name is both unreliable and prejudicial.",
    "What the dependency does at runtime. Nothing above is a substitute for reducing blast radius.",
]


TIER_REMINDERS = {
    "A": "TIER A - SHIPS TO USERS. Highest scrutiny. Pin hashes, record provenance, prefer building "
         "from source. A compromise here reaches everyone who installs your software.",
    "B": "TIER B - BUILDS THE PRODUCT. Pin versions, no floating fetches. A compromise here can "
         "inject into artifacts you ship.",
    "C": "TIER C - DEV-ONLY / SCRATCH DATA. Reduce blast radius instead of agonising over trust: no "
         "system-wide install, no PATH, no network, run it against a scratch directory. If it were "
         "malicious it would be malicious at a folder of temporary files.",
}


def main():
    parser = argparse.ArgumentParser(description="Gather the checkable facts about a dependency.")
    parser.add_argument("--repo", required=True, help="OWNER/NAME that publishes the artifact")
    parser.add_argument("--upstream", help="OWNER/NAME of the project it is built from")
    parser.add_argument("--author", help="Author name as it appears in upstream commits")
    parser.add_argument("--manifest-url", help="Raw package-manifest URL to check for hash pinning")
    parser.add_argument("--tier", choices=["A", "B", "C"], help="Blast-radius tier (see policy doc)")
    parser.add_argument("--json", action="store_true", help="Emit raw findings as JSON")
    args = parser.parse_args()

    findings = {"errors": []}

    probe_publisher(args.repo, findings)
    probe_releases(args.repo, findings)
    if args.upstream and args.author:
        probe_tenure(args.upstream, args.author, findings)
    elif args.upstream or args.author:
        findings["errors"].append("--upstream and --author must be given together for tenure checks")
    if args.manifest_url:
        probe_manifest(args.manifest_url, findings)

    flags, good = evaluate_flags(findings)

    if args.json:
        findings["flags"] = flags
        findings["reassuring"] = good
        findings["unanswerable"] = UNANSWERABLE
        print(json.dumps(findings, indent=2, sort_keys=True))
        return 0

    print("")
    print("=" * 78)
    print(" Dependency vetting: %s" % args.repo)
    print("=" * 78)

    if args.tier:
        print("")
        print(TIER_REMINDERS[args.tier])

    publisher = findings.get("publisher")
    if publisher:
        print("")
        print("PUBLISHER")
        print("  repo          %s%s" % (publisher["repo"], " (FORK)" if publisher.get("is_fork") else ""))
        print("  created       %s  (%s ago)" % (publisher.get("created_at"), years(publisher.get("age_days"))))
        print("  last push     %s  (%s days ago)" % (publisher.get("pushed_at"), publisher.get("days_since_push")))
        print("  license       %s" % (publisher.get("license"),))
        print("  stars         %s" % (publisher.get("stars"),))

    releases = findings.get("releases")
    if releases:
        print("")
        print("RELEASES")
        for key in sorted(releases):
            print("  %-13s %s" % (key, releases[key]))

    tenure = findings.get("tenure")
    if tenure:
        print("")
        print("MAINTAINER TENURE  (vs %s)" % tenure["upstream"])
        print("  author        %s" % tenure["author"])
        print("  first commit  %s  (%s ago)" % (tenure.get("first_commit"), years(tenure.get("tenure_days"))))
        print("  commits       %s" % (tenure.get("total_commits"),))
        print("  authority lag %s between first upstream commit and creating the distribution repo" % years(tenure.get("authority_lag_days")))

    manifest = findings.get("manifest")
    if manifest:
        print("")
        print("PACKAGE MANIFEST")
        print("  pins sha256   %s" % manifest.get("pins_sha256"))
        print("  pins sha512   %s" % manifest.get("pins_sha512"))
        for line in manifest.get("referenced_urls", []):
            print("  ref           %s" % line)

    print("")
    print("REASSURING")
    if good:
        for item in good:
            print("  + %s" % item)
    else:
        print("  (nothing)")

    print("")
    print("FLAGS")
    if flags:
        for item in flags:
            print("  ! %s" % item)
    else:
        print("  (none triggered)")

    print("")
    print("CANNOT BE ANSWERED BY THIS TOOL - record these as gaps, do not omit them")
    for item in UNANSWERABLE:
        print("  ? %s" % item)

    if findings["errors"]:
        print("")
        print("LOOKUP ERRORS  (an unchecked item is a GAP, not a pass)")
        for item in findings["errors"]:
            print("  x %s" % item)

    print("")
    print("No score is produced by design. Decide, record the decision and its date, and re-check")
    print("when the version changes. See doc/#Devel/Dependency-Vetting.md")
    print("")

    return 0


if __name__ == "__main__":
    sys.exit(main())
