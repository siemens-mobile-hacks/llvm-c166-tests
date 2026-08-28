# Configuration

All paths are optional environment overrides; the source tree contains no
developer-specific absolute path.

| Variable | Meaning | Default |
|---|---|---|
| `LLVM_BUILD` | C166 LLVM build | `/tmp/codex/llvm-c166-mc-build` |
| `LLVM_SOURCE` | LLVM source for coverage/fatal-path audits | sibling `llvm-project` |
| `TASKING_C166_WINEPREFIX` | Wine prefix used for execution and discovery | `${WINEPREFIX:-$HOME/.wine}` |
| `TASKING_C166_ROOT` | direct TASKING install root | discovered in selected prefix |
| `TASKING_C166_INSTALL_PREFIX` | prefix used only for install discovery | selected Wine prefix |
| `C166_TEST_JOBS` | CTest workers | `8`, forced to `1` for system Wine |
| `C166_TEST_CMAKE_BUILD` | CMake/CTest build directory | `build/cmake` |
| `C166_TEST_ARTIFACT_ROOT` | retained run artifacts | `build` |
| `C166_TEST_TMP_ROOT` | generated scratch data | `/tmp/codex` |

The maintained suite never writes to an LLVM source checkout or a TASKING
installation. Generated sources, CMake state, Wine session files, ELF/HEX/ABS
images, logs, and reports live under the artifact or scratch roots.

For normal unattended parallel work:

```sh
LLVM_BUILD=/path/to/llvm-build \
TASKING_C166_WINEPREFIX="$HOME/.wine-headless" \
./run -j 8 smoke
```

Use `--visible` only to inspect CrossView manually. Direct `ctest` execution is
not a supported headless entry point because it bypasses the suite-wide Xvfb
and Wine lifecycle wrappers.
