# Rules catalog

A catalog of the game calculations that rules profiles decide (see *Rules profiles* in
[doc/@Devel/Modding.md](../@Devel/Modding.md)): what each one is, where the C code asks for it, its inputs, and its
result for a set of named data points under every profile.

| File | What it is |
|---|---|
| [rules_catalog.ttl](rules_catalog.ttl) | The catalog as RDF triples (Turtle), one triple per line. Load it into any triple store or RDF library. |
| [rules_catalog.md](rules_catalog.md) | The same data as comparison tables: one table per calculation, one column per profile. |

Both files are **generated**; do not edit them:
- the calculations and data points come from [assets/rules/catalog.lua](../../assets/rules/catalog.lua);
- the values come from running each data point through each profile in [assets/rules/](../../assets/rules/);
- each profile's status, source and note come from its `rules.about(...)` calls.

The tests `RulesCatalog_Triples_UpToDate` and `RulesCatalog_Table_UpToDate` fail when either file no longer matches
the code. To regenerate both:

```bash
./out/build/clang-debug/bin/Debug/rules_catalog assets/rules/catalog.lua doc/rules-catalog/rules_catalog.ttl \
    assets/rules/original assets/rules/community_patch assets/rules/com2
python3 tools/rules_catalog/ttl_to_table.py doc/rules-catalog/rules_catalog.ttl > doc/rules-catalog/rules_catalog.md
```

To add a mod's profile as a column, pass its rules folder as one more profile. `--csv` gives a spreadsheet instead of
Markdown.

## Vocabulary

Namespace `remom:` = `https://github.com/jbalcomb/remom/ns/rules#`. The file also declares these terms itself.

| Class | Instances | Main properties |
|---|---|---|
| `remom:Calculation` | `calc:<rule>` | `remom:rule`, `rdfs:label`, `rdfs:comment`, `remom:unit`, `remom:cFunction`, `remom:cOriginal`, `remom:cFile`, `remom:wzd`, `remom:drake178`, `remom:input` |
| `remom:Input` | `input:<rule>-<name>` | `rdfs:label`, `rdfs:comment`, `remom:position` |
| `remom:DataPoint` | `dp:<rule>-<id>` | `remom:calculation`, `rdfs:label`, `remom:argument` |
| `remom:Profile` | `profile:<id>` | `rdfs:label`, `rdfs:comment`, `remom:base` |
| `remom:Value` | `val:<rule>-<id>-<profile>` | `remom:dataPoint`, `remom:profile`, `remom:value` |
| (profile statement) | `val:<rule>-about-<profile>` | `remom:about`, `remom:profile`, `remom:status`, `remom:source`, `remom:note` |

A value is its own node, linking one data point to one profile. That is the usual way to express a three-way relation
(data point × profile → result) in triples, and it lets a table be any pivot of it.

## Example query (SPARQL)

Every data point of every calculation, with its result per profile:

```sparql
PREFIX remom: <https://github.com/jbalcomb/remom/ns/rules#>
PREFIX rdfs:  <http://www.w3.org/2000/01/rdf-schema#>

SELECT ?calculation ?dataPoint ?profile ?value WHERE {
    ?v a remom:Value ;
       remom:dataPoint ?dp ;
       remom:profile ?p ;
       remom:value ?value .
    ?dp remom:calculation ?c ;
        rdfs:label ?dataPoint .
    ?c rdfs:label ?calculation .
    ?p rdfs:label ?profile .
}
ORDER BY ?calculation ?dataPoint ?profile
```

Only the differences from 1.31: add `?o a remom:Value ; remom:dataPoint ?dp ; remom:profile <https://github.com/jbalcomb/remom/ns/rules/profile/original> ; remom:value ?original . FILTER(?value != ?original)`.
