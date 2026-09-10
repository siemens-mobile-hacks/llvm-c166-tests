# C166 LLVM tests

## Dependencies

- Bash, GNU coreutils and a native C compiler (`cc`);
- CMake 3.20 or newer, including CTest;
- Python 3 with the packages from `requirements.txt`;
- `jq` and `rg`;
- Wine, including `wine`, `wineserver` and `winepath`;
- Xvfb, including `xvfb-run`;
- a C166 LLVM build containing `clang`, `ld.lld`, `llc`, `llvm-ar`, `llvm-objcopy`,
  `llvm-objdump`, `llvm-nm`, `llvm-readobj` and `llvm-size`;
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

Run the LLVM/TASKING size comparison for all memory models:

```sh
LLVM_BUILD=/path/to/llvm-build ./run '^size\.'
cat build/reports/large/code-size.md
cat build/reports/medium/code-size.md
cat build/reports/small/code-size.md
```

Run it for one model:

```sh
LLVM_BUILD=/path/to/llvm-build ./run '^size\.large\.codegen\.corpus$'
```

Check the linked compiler-runtime size limits for one model:

```sh
LLVM_BUILD=/path/to/llvm-build ./run '^size\.large\.runtime\.closure$'
```

Simulator test names have the form
`sim.<model>[.<variant>].<category>.<test>.<optimization>`. The default
variant is `ext` and is omitted from the name; `ext2` is explicit.

## Tests

`L`, `M` and `S` mean Large, Medium and Small. `all` means
`O0,O1,O2,O3,Os,Oz`.

| Category | Test | Models | Optimizations | Variant / extra run |
|---|---|---:|---|---|
| ABI | `abi-aggregate-args` | L/M/S | all | ext |
| ABI | `abi-aggregate-layout` | L | all | ext |
| ABI | `abi-aggregate-return` | L/M/S | all | ext; direct/indirect aggregate returns, mixed live values across callbacks, canaries, register/stack preservation |
| ABI | `abi-byte-return` | L/M | all | ext |
| ABI | `abi-code-bank` | L/S | all | ext |
| ABI | `abi-combined-matrix` | L/M/S | O0,O2,Oz | ext |
| ABI | `abi-far-automatic-matrix` | L | all | ext |
| ABI | `abi-far-memory-matrix` | L | all | ext |
| ABI | `abi-far-pointer` | L/M/S | all | ext |
| ABI | `abi-generated-aggregate` | L/M/S | all | ext |
| ABI | `abi-global-data-lifecycle` | L/M/S | all | ext |
| ABI | `abi-global-data-relocated` | L/M/S | all | ext |
| ABI | `abi-global-function-pointer` | L/M | all | ext |
| ABI | `abi-linked-application` | L/M/S | O0,O2,Oz | ext; multi-object link, archive extraction, weak/common, section GC, function table and host callback |
| ABI | `abi-linked-application-relocated` | L/M/S | O0,O2,Oz | ext; same image at a second text/data placement |
| ABI | `abi-fp-calls` | L/M/S | all | ext; mixed float/double C arguments and returns, both directions, direct/indirect, exact bytes |
| ABI | `abi-huge-pointer` | L/M | all | ext |
| ABI | `abi-indirect-call` | L/M/S | all | ext |
| ABI | `abi-indirect-matrix` | L/M/S | all | ext |
| ABI | `abi-interrupt` | L/M/S | all | ext |
| ABI | `abi-long-long` | L | all | ext |
| ABI | `abi-medium-function-classes` | M | all | ext |
| ABI | `abi-mixed-recursion` | L/M/S | all | ext; alternating LLVM/TASKING recursion, direct/indirect calls, float/aggregate/pointers and stack/register preservation |
| ABI | `abi-mixed-slot-matrix` | L/M/S | all | ext |
| ABI | `abi-multi-epilog` | L/S | all | ext |
| ABI | `abi-near-function` | L/S | all | ext |
| ABI | `abi-near-pointer` | L/M | all | ext |
| ABI | `abi-nested-call-chains` | L/M/S | all | ext |
| ABI | `abi-pair-r12` | L/M/S | all | ext |
| ABI | `abi-pointer-casts` | L/S | all | ext |
| ABI | `abi-qualified-globals` | L/M/S | all | ext |
| ABI | `abi-recursion-state` | L/M/S | all | ext |
| ABI | `abi-register-bank` | L/M/S | all | ext |
| ABI | `abi-register-pressure` | L/M/S | all | ext |
| ABI | `abi-reverse-stop` | L/M/S | all | ext |
| ABI | `abi-scalar` | L/M/S | all | ext |
| ABI | `abi-stack-page-limit` | L | all | ext |
| ABI | `abi-stack-stop` | L/M/S | all | ext |
| ABI | `abi-stackparm` | L/M/S | all | ext |
| ABI | `abi-tuple-reload` | L/M/S | all | ext |
| ABI | `abi-varargs` | L/M/S | all | ext |
| ABI | `abi-varargs-aggregate` | L/M/S | all | ext |
| ABI | `abi-varargs-matrix` | L/M/S | all | ext |
| ABI | `abi-word-matrix` | L/M/S | all | ext |
| ABI | `farptr` | L/M/S | O2 | ext |
| ABI | `regargs` | L/S | O2 | ext |
| ABI | `reverse` | L/M/S | O2 | ext |
| ABI | `saved` | L/S | O2 | ext |
| ABI | `stack` | L/M/S | O2 | ext |
| ABI | `structret` | L/M/S | O2 | ext |
| Codegen | `aggregate` | L/M/S | O2 | ext |
| Codegen | `alu-direct-memory` | L/M/S | O2 | ext/ext2; 290816 ALU/compare-and-loop result/flags cases, GPR/SFR register/memory/immediates, word/byte indirect addressing and postincrement |
| Codegen | `c-assembly` | L/M/S | all | ext; shared C and preprocessed assembly, C↔assembly calls, macros, local labels, data/BSS |
| Codegen | `backend-tuple-overlap` | L/S | O2 | ext |
| Codegen | `branch` | L/M/S | O2 | ext |
| Codegen | `code-segment-boundaries` | L/M/S | O2 | ext/ext2; JMPR/JMPA/JB IP-wrap and code-segment fetch boundaries |
| Codegen | `computed-goto` | L/M | all | ext |
| Codegen | `conditional-transfers` | L/M/S | O2 | ext/ext2; 3072 condition/flags/stack cases |
| Codegen | `extended-addressing` | L/M/S | O2 | ext/ext2; 128 EXT read/store cases plus 64 nested EXTP/EXTS cases, counts 1..4, counter reload and expiry |
| Codegen | `esfr-addressing` | L/M/S | O2 | ext/ext2; 68 SFR/ESFR routing/expiry cases plus 1280 EXT/ATOMIC flags cases; bit operations, MOV writes, PUSH reads, stack balance, counts 1..4 |
| Codegen | `external-branch-relaxation` | L/S | O2 | ext |
| Codegen | `jump-table` | L/M/S | O1,O2,O3,Os,Oz | ext |
| Codegen | `near-call-stack` | L/M/S | O2 | ext/ext2; CALLR/PCALL/RETP stack and flags |
| Codegen | `mov-memory` | L/M/S | O2 | ext/ext2; 5760 SFR/direct/indirect transfer/flags cases, byte predecrement, immediate and neighbor preservation |
| Codegen | `mov-indirect` | L/M/S | O2 | ext/ext2; 248064 word/byte load/store/copy cases, predecrement/postincrement, displacement, RL/RH registers, even/odd byte addresses, flags and source/neighbor preservation |
| Codegen | `mov-extend` | L/M/S | O2 | ext/ext2; 81920 MOVBS/MOVBZ result/flags cases, all byte values, GPR/SFR/memory and source preservation |
| Codegen | `bit-writeback` | L/M/S | O2 | ext/ext2; 49152 JBC/JNBS cases, GPR/RAM boundaries, flags, short/long branches and neighbor preservation |
| Codegen | `bit-branch` | L/M/S | O2 | ext/ext2; 49152 JB/JNB cases, taken/not-taken, operand/flags preservation, GPR/RAM, short/long branches; shared sources with bit-writeback |
| Codegen | `bit-binary` | L/M/S | O2 | ext/ext2; 61440 BMOV/BMOVN/BAND/BOR/BXOR/BCMP result/flags cases, GPR/RAM aliasing, source and neighbor preservation |
| Codegen | `bit-update` | L/M/S | O2 | ext/ext2; 24576 BSET/BCLR result/flags cases, all bit indices, GPR/RAM boundaries and neighbor preservation |
| Codegen | `bit-field` | L/M/S | O2 | ext/ext2; 49152 BFLDL/BFLDH mask/data/result/flags cases, GPR/RAM boundaries, other-byte and neighbor preservation |
| Codegen | `trap` | L/M/S | O2 | ext/ext2; 1536 segmented TRAP/RETI cases, three vectors, flags/priority, stack frame and CSP/SP restoration |
| Codegen | `protocol` | L/M/S | all | ext |
| Codegen | `shift-rotate` | L/M/S | O2 | ext/ext2; 103680 SHL/SHR/ASHR/ROL/ROR result/flags cases, count masking, sign-fill and destination/count aliasing |
| Differential | `control-flow-matrix` | L/M/S | all | ext |
| Differential | `far-pointer-alias-matrix` | L/M/S | all | ext |
| Differential | `float-varargs-matrix` | L/M/S | all | ext; bidirectional C variadic calls, float promotion, five fixed-argument shapes, exact FP words |
| Differential | `float-volatile-matrix` | L/S | all | ext |
| Differential | `float-width-matrix` | L/M/S | all | ext |
| Differential | `float32-arithmetic-matrix` | L | all | ext |
| Differential | `float32-conversion-matrix` | L/M/S | all | ext |
| Differential | `float32-storage-matrix` | L | all | ext |
| Differential | `float64-arithmetic-matrix` | L/M/S | all | ext |
| Differential | `float64-conversion-matrix` | L/M/S | all | ext |
| Differential | `float64-storage-matrix` | L | all | ext |
| Differential | `float64-snapshot` | L/M/S | all | ext |
| Differential | `float64-tasking-reverse` | L | all | ext |
| Differential | `generated-type-semantics` | L/M/S | all | ext |
| Differential | `integer16-matrix` | L/M/S | all | ext |
| Differential | `integer32-matrix` | L/M/S | all | ext |
| Differential | `memory-builtins` | L/M/S | all | ext |
| Differential | `metamorphic-conversion-matrix` | L/M/S | all | ext |
| Differential | `pseudo-expansion-matrix` | L/M/S | all | ext |
| Differential | `scalar-float-bytes` | L/M/S | all | ext |
| Differential | `stream-parser` | L/M/S | O0/O2/Oz | ext; 20 streaming parser/ring-buffer cases, chunking, wrap, overflow, malformed and partial frames |
| Differential | `callback-state-machine` | L/M/S | O0/O2/Oz | ext; 16 state-machine cases, indirect handlers, context callbacks, callback replacement and unsigned wrap |
| Differential | `record-codec` | L/M/S | O0/O2/Oz | ext; 28 string/record scenarios, exact wire bytes, unaligned buffers, all short capacities/truncations and malformed input |
| Runtime | `atomic-runtime` | L/S | all | ext |
| Runtime | `atomic-runtime-medium` | M | O1,O2,O3,Os,Oz | ext |
| Runtime | `atomic-runtime-medium-o0-aggregate` | M | O0 | ext |
| Runtime | `atomic-runtime-medium-o0-floating` | M | O0 | ext |
| Runtime | `atomic-runtime-medium-o0-integer` | M | O0 | ext |
| Runtime | `atomic-runtime-medium-o0-locking` | M | O0 | ext |
| Runtime | `clz32-matrix` | L/M/S | all | ext |
| Runtime | `i64-comparison-matrix` | L/M/S | all | ext |
| Runtime | `runtime` | L/M/S | O2 | ext |
| Runtime | `runtime-helper-matrix` | L/M/S | O2 | ext |
| Runtime | `tasking-runtime-variants` | L/S | O2 | ext/ext2 |
| Stress | `generated-cfg-pressure` | L/S | O2 | ext; `basic` |
| Stress | `seeded-abi-call` | L/S | all | ext; `10k` remains Large |
| Stress | `seeded-f32-differential` | L/M/S | all | ext; `100k` |
| Stress | `seeded-f64-add` | M | all | ext |
| Stress | `seeded-f64-compare` | M | all | ext |
| Stress | `seeded-f64-differential` | L/S | all | ext; `100k` |
| Stress | `seeded-f64-div` | M | all | ext |
| Stress | `seeded-f64-mul` | M | all | ext |
| Stress | `seeded-f64-to-f32` | L/M/S | all | ext; `100k` |
| Stress | `seeded-f64-sub` | M | all | ext |
| Stress | `seeded-far-pointer` | L | all | ext; `10k` |
| Stress | `seeded-integer-differential` | L/M/S | all | ext; Large `100k` |
| Size | `code-size-corpus` | L/M/S | LLVM Os / TASKING O2 | generated source-object size; external runtimes excluded |
| Size | `code-size-floating` | L/M/S | Os | shared float32/binary64 arithmetic, conversion and comparison corpus with linked runtime |
| Size | `runtime-closure` | L/M/S | Os | maximum linked ROM footprint for selected runtime calls |
