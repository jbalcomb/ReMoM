# Plan: IDA Pro 5.5 as a REST service on the Coder Workstation (Windows 10), run by Servy

Status: plan, nothing built. Branch `remom-mod`.

## Goal

Other machines (the Ubuntu AI Workstation, a cloud session attached through Remote Control) ask the Coder
Workstation to run an IDC script against one of its IDA 5.5 databases and get the result back as TSV, instead of
running IDA under Wine themselves. The first job is `export_map.idc`; any other script added to the scripts folder
later works the same way.

## Design

The service builds on what [tools/mom_patch_diff/export_map.py](../../tools/mom_patch_diff/export_map.py) already
does: it copies the database, runs `idag.exe`/`idaw.exe -A -S<script> -L<log>` on the copy, and checks the output.

- **Server:** `tools/ida_service/ida_service.py`, standard library only (`http.server.ThreadingHTTPServer`, a
  `queue.Queue`, one worker thread). IDA 5.5 runs one job per process; jobs run one at a time.
- **No paths or code from the client.** A request names a database and a script by *name* only:
  - databases from a configured folder (`WIZARDS`, `MAGIC`, later one per version);
  - scripts from `tools/ida_service/scripts/` in the repo.

  IDC can run programs (`Exec`), so accepting uploaded IDC would mean accepting arbitrary code. To add a script,
  commit it.
- **Never touch the originals:** every job runs on its own copy of the `.idb` and its companions, as
  `export_map.py` does now.
- **Job folder:** `jobs/<id>/` under the service's data folder holds `job.json` (state, times, exit code), the
  output TSV and `ida.log`. Jobs older than a configured number of days are deleted. When the service restarts,
  jobs it left `running` are marked `failed`.
- **Timeout:** each job has a timeout (default 1800 s). After it, the IDA process is killed and the job fails with
  the last lines of the log.

### Endpoints

| Method | Path | Returns |
|---|---|---|
| `GET` | `/health` | service version, IDA path, databases and scripts available, queue length |
| `POST` | `/jobs` | body `{"idb": "WIZARDS", "script": "export_map"}`; returns `{"id": ...}` |
| `GET` | `/jobs/<id>` | `job.json` |
| `GET` | `/jobs/<id>/output` | the TSV |
| `GET` | `/jobs/<id>/log` | IDA's log |

### Access

- The server binds to `127.0.0.1` by default.
- To serve the LAN: bind to the workstation's LAN address and require a bearer token (stored in the config file,
  never in the repo). Then add one Windows Firewall rule that allows the port **only from the AI Workstation's
  address**.
- An SSH tunnel through Windows 10's OpenSSH server is the alternative that needs neither the open port nor the
  token.

### Client

- `tools/ida_service/ida_client.py`, standard library only: `submit`, `wait`, `fetch`.
- `export_map.py --service URL` uses the client, so the address table and patch index steps in
  [Handoff-remom-mod.md](Handoff-remom-mod.md) work the same whether IDA is local, under Wine, or on the Coder
  Workstation.

### Code changes

- `export_map.py`: move the run into a function, `run_ida(ida, idb, script_text, out, timeout, wine)`. Its
  `main()` and the service both call it.
- The script-file handling (the `@@OUTPUT_PATH@@` placeholder) stays as it is.

## Running it as a Windows service with Servy

[Servy](https://github.com/aelassas/servy) (MIT, Akram El Assas) runs any program as a Windows service. It handles
start at boot, restart on failure, stdout/stderr logs with rotation, a service account, and pre/post start and stop
hooks. It has a GUI, a CLI (`servy-cli`) and a PowerShell module. Only the CLI is used here.

Servy replaces NSSM, which the first draft of this plan used. "Superseded" is the author's own claim, but on this
project's [vetting policy](../@Devel/Dependency-Vetting.md) the comparison favours Servy for two reasons:

- **Build transparency** (the check that would have caught xz).
  - Servy: built in public GitHub Actions; binaries signed through the SignPath Foundation, which signs builds from
    a project's own CI.
  - NSSM: last released in 2017, unsigned, no public build.
- **Maintenance.** Servy is active (6,684 commits, 2.1k stars when checked); NSSM is unmaintained.

WinSW stays the fallback if Servy fails vetting.

### Vetting (do this first, on the workstation)

- **Tier C:** dev-only, never ships, never builds the product. It is *not* offline scratch, though. It runs a
  program at boot, under a service account, that listens on the network.
- **Verified 2026-11-02 from the repository page and wiki:**
  - MIT licence;
  - GitHub Actions builds;
  - "all releases feature signed binaries" (SignPath);
  - the CLI options used below, including `--user`, which supports custom accounts per service from v10.2.
- **Not checked.** This cloud session's GitHub API calls were refused, so `tools/vet_dependency.py` got nothing.
  Run it on the workstation with a `GITHUB_TOKEN` and record:
  - the repository's age and the date of the first release;
  - the maintainer's tenure, against the date the signing channel was set up;
  - the release you install, and that its signature names the SignPath Foundation;
  - the .NET runtime it needs.
- **Gaps that stay open:**
  - **Single maintainer:** a stolen account or signing setup is not covered by tenure.
  - **Much bigger than NSSM:** a GUI, email and notification code, an external heartbeat URL. Use only the CLI,
    and leave the notification, email and heartbeat-URL options unset, so none of that code has a job to do.
  - **Stop behaviour not documented:** the wiki says nothing on whether Servy sends Ctrl+C or kills the process
    tree on stop. `--stopTimeout` is the only stop setting. The service must not depend on either (see below).

### Containing it

- **Install:** the pinned, signed release. Record its version and SHA-256 in the tracked
  `tools/ida_service/servy_manifest.tsv`. `install_service.sh` refuses to run if the installed `servy-cli.exe`
  differs.
- **Account:** a dedicated local account (`idasvc`), not Local System, which is what Servy uses when `--user` is
  omitted.
  - `idasvc` can read the IDA install, the database folder and the repo checkout, and write only the service's
    data folder.
  - Log in as `idasvc` once, interactively, and start IDA by hand. IDA 5.5 asks its first-run questions (licence
    agreement) per user. In a service those dialogs would hang where nobody can see them.
- **Use `idaw.exe` (text mode), not `idag.exe`.** Services run in session 0 and have no visible desktop, so any
  dialog IDA opens there waits until the job's timeout. The text-mode IDA has less to open.
- **Python 3:** use the absolute path of the Python 3 interpreter. On this project's Windows boxes `python` is 2.7
  (see [AGENTS.md](../../AGENTS.md)). The service also checks `sys.version_info` and exits with an error under
  Python 2.
- **IDA cannot outlive the service:**
  - The service starts each IDA in a Windows Job Object with `JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE` (through
    `ctypes`, standard library). However Servy stops the Python process, the IDA it started dies with it.
  - On startup, the service marks jobs left `running` as `failed`.
- **Leave health checks off:** Servy's `--heartbeatUrl` pings an *outside* URL. It does not call the service's own
  `/health`, so leave it unset. `--recoveryAction RestartProcess` covers crashes.

### Install (elevated Git Bash; `install_service.sh` will do this after the hash check)

```bash
PY3='C:/Users/<you>/AppData/Local/Programs/Python/Python312/python.exe'   # the Python 3 interpreter, absolute
export SERVY_PASSWORD='<idasvc password>'                                # keeps the password off the command line
servy-cli install --name="ReMoM-IDA" --displayName="ReMoM IDA service" \
    --path="$PY3" \
    --params="C:/STU/devel/ReMoM/tools/ida_service/ida_service.py --config C:/STU/ida_service/config.json" \
    --startupDir="C:/STU/ida_service" \
    --startupType="AutomaticDelayedStart" \
    --user='.\idasvc' \
    --stdout="C:/STU/ida_service/logs/service_stdout.log" \
    --stderr="C:/STU/ida_service/logs/service_stderr.log" \
    --enableSizeRotation --rotationSize=10 --maxRotations=10 \
    --recoveryAction="RestartProcess" --maxRestartAttempts=5 \
    --stopTimeout=30
unset SERVY_PASSWORD
servy-cli start --name="ReMoM-IDA"
curl -s http://127.0.0.1:8765/health
```

- `--params` uses `=` as the wiki requires when the value holds `--` arguments.
- Windows paths use the `C:/...` form, because Git Bash rewrites `/c/...` only when the whole argument is one path.
- The exact boolean-flag syntax (`--enableSizeRotation` alone or `=true`) is unverified. Check it against
  `servy-cli install --help` for the installed version.

## Phases

1. **Service on Linux, tested with the fake IDA.**
   - Write `run_ida()`, `ida_service.py` and `ida_client.py`.
   - Unit tests start the server on an ephemeral port with the fake `idag` from
     `tools/mom_patch_diff/tests/test_address_table.py`.
   - Cover: submit, poll, fetch, unknown database or script names (rejected), timeout, restart marking jobs failed,
     token check.
   - This phase can be done in a cloud session.
2. **By hand on the Coder Workstation.** Run `python3 tools/ida_service/ida_service.py` in a console with the real
   IDA 5.5 and `WIZARDS.idb`.
   - This is also the first real run of `export_map.idc`, which has only ever run against the fake.
3. **Servy.**
   - Vet it and record the decision.
   - Create `idasvc` and do its first IDA run.
   - Run `install_service.sh`, reboot, and check that `/health` answers.
   - Stop the service during a job and check that no `idaw.exe` is left running.
4. **From the AI Workstation.**
   - Add the token and the firewall rule (or the SSH tunnel).
   - Run `export_map.py --service` for both databases, then the address table.
   - Update [Handoff-remom-mod.md](Handoff-remom-mod.md).

## Open questions

- **Port:** 8765 is a placeholder.
- **Several IDA versions:** one database folder per game version means one service with a `version` field in the
  request, rather than one service per version.
