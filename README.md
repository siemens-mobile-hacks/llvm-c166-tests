# C166 LLVM tests

This repository validates LLVM C166 ELF code against the C166 C ABI by running
LLVM and TASKING-built components together in the TASKING instruction-set
simulator. It covers Large, Medium, and Small memory models with one shared
proxy, memory layout, startup policy, and result protocol.

## Requirements

- a C166-only LLVM build containing `clang`, `llc`, LLD and LLVM binary tools;
- TASKING C166 8.6 with `cstart`, model-specific `c166`, `fp166`, and `rt166`
  libraries, and CrossView;
- Wine, Xvfb, CMake 3.20+, Python 3, jq, ripgrep, and a native C compiler.

The default LLVM build is `/tmp/codex/llvm-c166-mc-build`. TASKING is located
inside the selected Wine prefix unless `TASKING_C166_ROOT` is set.

## Run

```sh
./run --list
./run smoke
./run deterministic
./run differential
./run optimization
./run full
./run 'sim.large.abi.regargs.O2'
```

Headless mode is the default and uses one Xvfb server for the entire CTest run.
`./run --visible FILTER` is reserved for interactive debugging. The system Wine
prefix is always serialized. An explicit private prefix may use the default
eight workers:

```sh
TASKING_C166_WINEPREFIX="$HOME/.wine-headless" ./run -j 8 smoke
```

Artifacts are retained under `build/`. A failed test prints its isolated run
directory; `run-state.json`, both linked images, disassembly, symbols, map,
session command stream, and simulator log remain there.

Useful inspection commands:

```sh
./tools/check-manifests
./tools/inventory
./tools/explain-case abi-far-pointer
./tools/generate-coverage-report
```

See [architecture](docs/architecture.md), [configuration](docs/configuration.md),
[manifest reference](docs/manifests.md), and [coverage](docs/coverage.md).
