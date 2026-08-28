# Architecture

## Contract under test

The suite tests LLVM-generated C166 ELF, not TASKING object-format
compatibility. TASKING C166 is the external C ABI oracle and supplies the host,
startup, model-specific runtime libraries, and simulator. LLVM supplies a
normal ELF image. The harness resolves any declared TASKING symbols from the
located host map, performs the final ELF link, converts both images to Intel
HEX, and overlays the LLVM ranges before one atomic simulator download.

## One universal harness

Every ordinary case uses `harness/simulator/proxy.asm`,
`harness/simulator/layout.ilo`, and the
same result validator. Cases cannot replace the proxy or layout. Installed
TASKING startup and libraries are authoritative; a missing or broken vendor
helper is a test failure, never replaced by a local compatibility shim.

The fixed image owns 2 MiB ROM at `0x100000..0x2fffff` and 2 MiB RAM at
`0x400000..0x5fffff`. Large and Small enter at `0x100000`, with ordinary text
at `0x180000`. Medium also owns `0xc000..0xefff` for near code. Qualified near
data uses `0x5000..0x5fff`; xnear data uses `0x6000..0x6fff`. ELF allocated
`SHT_NOBITS` ranges are explicitly zero-filled because Intel HEX cannot encode
them.

Memory-model mappings live only in `harness/linker/model.sh`. Tool discovery,
ELF-image construction, simulator-session preparation, and run-state tracking
are separate modules with Wine-free self-tests. `harness/simulator.sh` is the
linear orchestrator and records completed stages in each run's
`run-state.json`.

## Execution and isolation

`./run` configures the manifest-generated CTest registry. A headless run starts
one private Xvfb display around the complete process tree. Each configuration
uses its own artifact directory and Wine-side simulator files. The system Wine
prefix is limited to one CTest worker and its wineserver is not terminated.
For an explicitly selected private prefix, the wrapper owns only that prefix's
wineserver and parallel execution is allowed.

Tests fail on tool errors, unresolved imports or relocations, missing required
symbols, ELF structural mismatches, overlay errors, simulator diagnostics,
timeouts, and incorrect scalar or result-block values. Failure evidence is
never discarded.
