# Coverage and evidence

`./tools/inventory` derives case and configuration counts directly from
manifests. `./tools/generate-coverage-report` revalidates retained runs and
writes `build/reports/coverage.json` and `coverage.md`; historical prose is not
used as a pass claim.

A retained configuration counts as passing only when the common result-log
checker confirms image download and verification, rejects simulator
diagnostics, and matches the exact scalar or result-block oracle. The report
also inventories final opcodes, runtime symbols, relocations, and the newest
passing attempt separately from the newest failed attempt.

The registry contains 89 cases and 751 ordinary configurations: 405 Large,
195 Medium, and 151 Small. Manifests declare six additional generated stress
runs.

Validation tiers are:

- `smoke`: every declared O2 configuration;
- `deterministic`: O0, O2, and Oz;
- `optimization`: all O0/O1/O2/O3/Os/Oz configurations;
- `differential`: compiler-versus-oracle matrices;
- `stress`: declared long-seed runs;
- `full`: every simulator test and infrastructure self-test.

Machine-level coverage is evidence-based. Structural `expected.dis` and
`expected.nm` patterns verify required ABI paths in the final ELF, while
behavioral results remain the primary oracle. Control-flow self-tests verify
both outcomes and switch arms of their native reference corpus. Pseudo
provenance recompiles retained sources around `postrapseudos` and requires all
reachable C166 post-RA pseudos to disappear before object emission.

CrossView's optional GUI `ce/covinfo` facility is not used as LLVM coverage:
it reports TASKING symbols loaded from the host ABS but does not attribute the
overlaid LLVM ELF. A clean run therefore records this probe as `not_probed`
and relies on executable result evidence, final ELF structure, and MachineIR
provenance instead.
