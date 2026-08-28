# C166 LLVM tests

Simulator tests for LLVM C166 ELF code and the C166 C ABI. TASKING C166
provides the ABI oracle, startup, runtime libraries, and instruction-set
simulator. The suite covers Large, Medium, and Small memory models and both
C167 (`ext`) and Super10/C166Sv2 (`ext2`) runtime variants.

## Requirements

- a C166 LLVM build with Clang, LLD, `llc`, and LLVM binary tools;
- TASKING C166 8.6 with CrossView, both `cstart` sources, and `c166`, `fp166`,
  and `rt166` libraries for every tested model;
- Wine, Xvfb, CMake 3.20+, Python 3, jq, ripgrep, and a native C compiler;
- Python packages from `requirements.txt`.

The LLVM build defaults to the sibling `llvm-project/build` directory. The
selected Wine prefix is used for TASKING discovery. Override them when needed:

```sh
python3 -m pip install -r requirements.txt
LLVM_BUILD=/path/to/llvm-build \
WINEPREFIX="$HOME/.wine-headless" \
./run -j 8 smoke
```

The system Wine prefix is always run serially. Headless mode uses one Xvfb
server for the complete run; use `--visible` only for simulator debugging.

## Commands

```sh
./run --list
./run smoke
./run deterministic
./run differential
./run optimization
./run stress
./run full
./run 'sim.large.abi.regargs.O2'
./tools/check-manifests
./tools/explain-case abi-far-pointer
./tools/generate-coverage-report
```

Failed-run artifacts remain under `build/`, including linked images,
disassembly, symbols, map, simulator commands and log, and `run-state.json`.

Reference: [configuration](docs/configuration.md),
[architecture](docs/architecture.md), [manifests](docs/manifests.md), and
[coverage](docs/coverage.md).
