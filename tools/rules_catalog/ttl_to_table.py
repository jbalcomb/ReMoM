#!/usr/bin/env python3
"""
ttl_to_table -- turns the rules catalog (RDF triples, doc/rules-catalog/rules_catalog.ttl) into comparison tables:
one table per calculation, one row per data point, one column per rules profile.  A value that differs from the
first profile's is shown in bold.

Usage:  python3 tools/rules_catalog/ttl_to_table.py doc/rules-catalog/rules_catalog.ttl > doc/rules-catalog/rules_catalog.md
        python3 tools/rules_catalog/ttl_to_table.py --csv doc/rules-catalog/rules_catalog.ttl > rules_catalog.csv

Reads the one-triple-per-line Turtle that tools/rules_catalog writes and stops on anything else, so a hand edit that
breaks the format is reported, not silently misread.  Any RDF tool can load the same file for other queries.
Standard library only.
"""

import csv
import re
import sys
from collections import defaultdict

TERM = r'(?:[A-Za-z][A-Za-z0-9_]*:[A-Za-z0-9_][A-Za-z0-9_.\-]*(?<!\.)|"(?:[^"\\]|\\.)*"|-?\d+)'
TRIPLE = re.compile(r'^(' + TERM + r') (' + TERM + r') (' + TERM + r') \.$')
PREFIX = re.compile(r'^@prefix [A-Za-z][A-Za-z0-9_]*: <[^>]+> \.$')


def literal(term):
    if term.startswith('"'):
        return re.sub(r'\\(.)', lambda m: '\n' if m.group(1) == 'n' else m.group(1), term[1:-1])
    return term


def read(path):
    subjects = defaultdict(lambda: defaultdict(list))
    with open(path, encoding='utf-8') as f:
        for number, line in enumerate(f, 1):
            line = line.rstrip('\n')
            if not line or line.startswith('#') or PREFIX.match(line):
                continue
            m = TRIPLE.match(line)
            if not m:
                sys.exit(f"{path}:{number}: not a one-line triple: {line}")
            subjects[m.group(1)][m.group(2)].append(m.group(3))
    return subjects


def first(props, predicate, default=''):
    values = props.get(predicate)
    return literal(values[0]) if values else default


def of_type(subjects, cls):
    return [s for s, p in subjects.items() if cls in p.get('rdf:type', [])]


def main(argv):
    as_csv = '--csv' in argv
    paths = [a for a in argv[1:] if a != '--csv']
    if len(paths) != 1:
        sys.exit(__doc__)
    g = read(paths[0])

    profiles = of_type(g, 'remom:Profile')                      # file order = the order given to rules_catalog
    calculations = of_type(g, 'remom:Calculation')
    points = defaultdict(list)
    for d in of_type(g, 'remom:DataPoint'):
        points[g[d]['remom:calculation'][0]].append(d)
    values = {}
    for v in of_type(g, 'remom:Value'):
        values[(g[v]['remom:dataPoint'][0], g[v]['remom:profile'][0])] = first(g[v], 'remom:value', first(g[v], 'remom:status', '?'))
    about = {}
    for s, p in g.items():
        if 'remom:about' in p:
            about[(p['remom:about'][0], p['remom:profile'][0])] = p

    if as_csv:
        w = csv.writer(sys.stdout)
        w.writerow(['calculation', 'data point', 'arguments'] + [first(g[p], 'rdfs:label', p) for p in profiles])
        for c in calculations:
            for d in points[c]:
                w.writerow([first(g[c], 'remom:rule'), first(g[d], 'rdfs:label'), first(g[d], 'remom:argument')]
                           + [values.get((d, p), '') for p in profiles])
        return

    out = ['# Rules catalog', '',
           'Generated from `doc/rules-catalog/rules_catalog.ttl` by `tools/rules_catalog/ttl_to_table.py`; do not edit.',
           'A **bold** value differs from the first profile\'s.', '', '## Profiles', '',
           '| Profile | Name | Starts from | Description |', '|---|---|---|---|']
    for p in profiles:
        base = first(g[p], 'remom:base').replace('profile:', '')
        out.append(f"| `{p.replace('profile:', '')}` | {first(g[p], 'rdfs:label')} | {base and '`' + base + '`'} | {first(g[p], 'rdfs:comment')} |")
    for c in calculations:
        props = g[c]
        out += ['', f"## {first(props, 'rdfs:label')} (`rules.{first(props, 'remom:rule')}`)", '',
                first(props, 'rdfs:comment'), '',
                f"Asked for by `{first(props, 'remom:cFunction')}()` in `{first(props, 'remom:cFile')}`; 1.31 original "
                f"`{first(props, 'remom:cOriginal')}()` (WZD {first(props, 'remom:wzd')}, drake178: `{first(props, 'remom:drake178')}`). "
                f"Unit: {first(props, 'remom:unit')}.", '']
        inputs = sorted(props.get('remom:input', []), key=lambda i: int(first(g[i], 'remom:position', '0')))
        out += [f"- `{first(g[i], 'rdfs:label')}`: {first(g[i], 'rdfs:comment')}" for i in inputs] + ['']
        out.append('| Data point | Arguments | ' + ' | '.join(first(g[p], 'rdfs:label', p) for p in profiles) + ' |')
        out.append('|---|---|' + '---|' * len(profiles))
        for d in points[c]:
            row = [values.get((d, p), '') for p in profiles]
            cells = [v if (i == 0 or v == row[0]) else f'**{v}**' for i, v in enumerate(row)]
            out.append(f"| {first(g[d], 'rdfs:label')} | `{first(g[d], 'remom:argument')}` | " + ' | '.join(cells) + ' |')
        notes = [(p, about[(c, p)]) for p in profiles if (c, p) in about]
        if notes:
            out += ['', '| Profile | Status | Source | Note |', '|---|---|---|---|']
            for p, a in notes:
                out.append(f"| {first(g[p], 'rdfs:label')} | {first(a, 'remom:status')} | {first(a, 'remom:source')} | {first(a, 'remom:note')} |")
    print('\n'.join(out))


if __name__ == '__main__':
    main(sys.argv)
