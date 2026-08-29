# C166 LLVM tests

## Dependencies

- Bash, GNU coreutils and a native C compiler (`cc`);
- CMake 3.20 or newer, including CTest;
- Python 3 with the packages from `requirements.txt`;
- `jq` and `rg`;
- Wine, including `wine`, `wineserver` and `winepath`;
- Xvfb, including `xvfb-run`;
- a C166 LLVM build containing `clang`, `ld.lld`, `llc`, `llvm-objcopy`,
  `llvm-objdump`, `llvm-nm` and `llvm-readobj`;
- TASKING C166 8.6 installed under
  `drive_c/Program Files (x86)/TASKING/dc166 v8.6` in the selected Wine
  prefix. The installation must contain CrossView, `_c_init.asm`, and the
  `c166`, `fp166` and `rt166` libraries for the tested memory models and
  `ext`/`ext2` variants.

```sh
python3 -m pip install -r requirements.txt
```

## Run all tests

`LLVM_BUILD` is required. `WINEPREFIX` is optional and defaults to
`$HOME/.wine`.

```sh
LLVM_BUILD=/path/to/llvm-build ./run full
```

With a separate Wine prefix:

```sh
LLVM_BUILD=/path/to/llvm-build \
WINEPREFIX="$HOME/.wine-headless" \
./run full
```

## Run one test

List matching CTest names:

```sh
LLVM_BUILD=/path/to/llvm-build ./run --list abi-scalar
```

Run one exact configuration:

```sh
LLVM_BUILD=/path/to/llvm-build \
./run '^sim\.large\.abi\.abi-scalar\.O2$'
```

Test names have the form
`sim.<model>[.<variant>].<category>.<test>.<optimization>`. The default
variant is `ext` and is omitted from the name; `ext2` is explicit.

## Tests

`L`, `M` and `S` mean Large, Medium and Small. `all` means
`O0,O1,O2,O3,Os,Oz`.

| Category | Test | Models | Optimizations | Variant / extra run |
|---|---|---:|---|---|
| ABI | `abi-aggregate-args` | L/M/S | all | ext |
| ABI | `abi-aggregate-layout` | L | all | ext |
| ABI | `abi-aggregate-return` | L/M/S | all | ext |
| ABI | `abi-byte-return` | L/M | all | ext |
| ABI | `abi-code-bank` | L | all | ext |
| ABI | `abi-combined-matrix` | L | O0,O2,Oz | ext |
| ABI | `abi-far-automatic-matrix` | L | all | ext |
| ABI | `abi-far-memory-matrix` | L | all | ext |
| ABI | `abi-far-pointer` | L/M/S | all | ext |
| ABI | `abi-generated-aggregate` | L | all | ext |
| ABI | `abi-global-data-lifecycle` | L/M/S | all | ext |
| ABI | `abi-global-function-pointer` | L/M | all | ext |
| ABI | `abi-huge-pointer` | L/M | all | ext |
| ABI | `abi-indirect-call` | L | all | ext |
| ABI | `abi-indirect-matrix` | L | all | ext |
| ABI | `abi-interrupt` | L/M | all | ext |
| ABI | `abi-long-long` | L | all | ext |
| ABI | `abi-medium-function-classes` | M | all | ext |
| ABI | `abi-mixed-recursion` | L | all | ext |
| ABI | `abi-mixed-slot-matrix` | L/M/S | all | ext |
| ABI | `abi-multi-epilog` | L | all | ext |
| ABI | `abi-near-function` | L/S | all | ext |
| ABI | `abi-near-pointer` | L/M | all | ext |
| ABI | `abi-nested-call-chains` | L/M/S | all | ext |
| ABI | `abi-pair-r12` | L | all | ext |
| ABI | `abi-pointer-casts` | L/S | all | ext |
| ABI | `abi-qualified-globals` | L/M/S | all | ext |
| ABI | `abi-recursion-state` | L/M/S | all | ext |
| ABI | `abi-register-bank` | L/M | all | ext |
| ABI | `abi-register-pressure` | L | all | ext |
| ABI | `abi-reverse-stop` | L | all | ext |
| ABI | `abi-scalar` | L/M/S | all | ext |
| ABI | `abi-stack-page-limit` | L | all | ext |
| ABI | `abi-stack-stop` | L | all | ext |
| ABI | `abi-stackparm` | L/M/S | all | ext |
| ABI | `abi-tuple-reload` | L | all | ext |
| ABI | `abi-varargs` | L/M/S | all | ext |
| ABI | `abi-varargs-aggregate` | L/M/S | all | ext |
| ABI | `abi-varargs-matrix` | L | all | ext |
| ABI | `abi-word-matrix` | L/M/S | all | ext |
| ABI | `farptr` | L | O2 | ext |
| ABI | `regargs` | L/S | O2 | ext |
| ABI | `reverse` | L | O2 | ext |
| ABI | `saved` | L | O2 | ext |
| ABI | `stack` | L | O2 | ext |
| ABI | `structret` | L | O2 | ext |
| Codegen | `aggregate` | L | O2 | ext |
| Codegen | `backend-tuple-overlap` | L | O2 | ext |
| Codegen | `branch` | L | O2 | ext |
| Codegen | `computed-goto` | L/M | all | ext |
| Codegen | `external-branch-relaxation` | L | O2 | ext |
| Codegen | `jump-table` | L/M/S | O1,O2,O3,Os,Oz | ext |
| Codegen | `protocol` | L | all | ext |
| Differential | `control-flow-matrix` | L | all | ext |
| Differential | `far-pointer-alias-matrix` | L/M/S | all | ext |
| Differential | `float-varargs-matrix` | L | all | ext |
| Differential | `float-volatile-matrix` | L | all | ext |
| Differential | `float-width-matrix` | L/M/S | all | ext |
| Differential | `float32-arithmetic-matrix` | L | all | ext |
| Differential | `float32-conversion-matrix` | L/M/S | all | ext |
| Differential | `float32-storage-matrix` | L | all | ext |
| Differential | `float64-arithmetic-matrix` | L | all | ext |
| Differential | `float64-conversion-matrix` | L/S | all | ext |
| Differential | `float64-storage-matrix` | L | all | ext |
| Differential | `float64-tasking-reverse` | L | all | ext |
| Differential | `generated-type-semantics` | L | all | ext |
| Differential | `integer16-matrix` | L | all | ext |
| Differential | `integer32-matrix` | L/S | all | ext |
| Differential | `metamorphic-conversion-matrix` | L/M/S | all | ext |
| Differential | `pseudo-expansion-matrix` | L | all | ext |
| Runtime | `atomic-runtime` | L/S | all | ext |
| Runtime | `atomic-runtime-medium` | M | O1,O2,O3,Os,Oz | ext |
| Runtime | `atomic-runtime-medium-o0-aggregate` | M | O0 | ext |
| Runtime | `atomic-runtime-medium-o0-floating` | M | O0 | ext |
| Runtime | `atomic-runtime-medium-o0-integer` | M | O0 | ext |
| Runtime | `atomic-runtime-medium-o0-locking` | M | O0 | ext |
| Runtime | `clz32-matrix` | L | all | ext |
| Runtime | `runtime` | L | O2 | ext |
| Runtime | `runtime-helper-matrix` | L/M/S | O2 | ext |
| Runtime | `tasking-runtime-variants` | L/S | O2 | ext/ext2 |
| Stress | `generated-cfg-pressure` | L | O2 | ext; `basic` |
| Stress | `seeded-abi-call` | L | all | ext; `10k` |
| Stress | `seeded-f32-differential` | L/M/S | all | ext; `100k` |
| Stress | `seeded-f64-add` | M | all | ext |
| Stress | `seeded-f64-differential` | L/S | all | ext; `100k` |
| Stress | `seeded-f64-div` | M | all | ext |
| Stress | `seeded-f64-mul` | M | all | ext |
| Stress | `seeded-f64-sub` | M | all | ext |
| Stress | `seeded-far-pointer` | L | all | ext; `10k` |
| Stress | `seeded-integer-differential` | L | all | ext; `100k` |
