# Case manifests

Cases live at `tests/<category>/<name>/case.json`; categories are `abi`,
`codegen`, `runtime`, `differential`, and `stress`. The JSON Schema is
`schema/case.schema.json`, currently version 2. CMake discovers every manifest
and creates stable names of the form:

```text
sim.<model>.<category>.<case>.<optimization>
```

Required fields declare LLVM and TASKING sources, the LLVM entry symbol,
optimizations, labels, runtime-builtin use, required ELF symbols, result
protocol, and outer timeout. `models` defaults to `large`; add `medium` and/or
`small` to reuse the same fixtures for another model.

Model-specific ABI evidence belongs in `required_symbols_by_model`,
`extra_ldflags_by_model`, or `result.expected_signature_by_model`. Do not copy
a case merely to change one of those properties. `source_case` is retained for
the ten reviewed variants that intentionally share a generated corpus while
using a different signature or placement contract.

`tasking_runtime` is `system` (the default) or `none`. System mode uses the
installed model-specific libc/floating/runtime libraries and installed
`cstartx.asm`; raw ABI boundaries use the common minimal startup. Local
runtime substitutes and case-specific proxy/layout files are forbidden.

`tasking_import_symbols` declares external ABI-oracle symbols referenced by
the LLVM ELF. The runner resolves them from the actual located TASKING map;
hard-coded numeric `_tasking_*` definitions are rejected.

Stress invocations use declarative `stress_runs` entries with a name, model,
runner, arguments, labels, and timeout. They are registered from the manifest,
not from hand-written CMake blocks.

To add a case:

1. Create `tests/<category>/<name>/` and its source fixtures.
2. Copy and edit a nearby schema-v2 manifest.
3. Run `./tools/validate-manifest path/to/case.json` and
   `./tools/check-manifests`.
4. Inspect `./tools/explain-case <name>` and `./run --list`.
5. Run the new O2 configuration, then all declared optimizations.

To extend a case to another model, add the model to `models`, declare only the
model-specific expectations that differ, and run every optimization already
declared by that case.
