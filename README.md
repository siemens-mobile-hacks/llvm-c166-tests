# C166 LLVM tests

## Dependencies

- Bash, GNU coreutils and a C++17 compiler;
- CMake 3.20 or newer, including CTest;
- Python 3;
- Wine, including `wine` and `wineserver`;
- Xvfb, including `xvfb-run`;
- a C166 LLVM build containing `clang`, `ld.lld`, `llc`, `llvm-ar`, `llvm-objcopy`,
  `llvm-objdump`, `llvm-nm` and `llvm-size`;
- TASKING C166 8.6 installed under
  `drive_c/Program Files (x86)/TASKING/dc166 v8.6` in the selected Wine
  prefix. The installation must contain CrossView, `_c_init.asm`, and the
  `c166`, `fp166` and `rt166` libraries for the tested memory models and
  `ext`/`ext2` variants.

## Run all tests

`LLVM_BUILD` is required. `WINEPREFIX` is optional and defaults to
`$HOME/.wine`.

```sh
LLVM_BUILD=/path/to/llvm-build ./run
```

The runner uses eight build and test workers by default; pass `-j N` to change
both limits.

With a separate Wine prefix:

```sh
LLVM_BUILD=/path/to/llvm-build \
WINEPREFIX="$HOME/.wine-headless" \
./run
```

## Run one test

List matching CTest names:

```sh
LLVM_BUILD=/path/to/llvm-build ./run --list abi-scalar
```

Run one exact configuration:

```sh
LLVM_BUILD=/path/to/llvm-build \
./run '^run\.large\.runtime\.bool-semantics\.llvm\.O2$'
```

Run the LLVM/TASKING size comparison for all memory models:

```sh
LLVM_BUILD=/path/to/llvm-build ./run '^size\.'
cat build/cmake/reports/large/code-size.md
cat build/cmake/reports/medium/code-size.md
cat build/cmake/reports/small/code-size.md
cat build/cmake/reports/tiny/code-size.md
cat build/cmake/reports/huge/code-size.md
```

Run it for one model:

```sh
LLVM_BUILD=/path/to/llvm-build ./run '^size\.large\.codegen\.corpus$'
```

Check the linked compiler-runtime size limits for one model:

```sh
LLVM_BUILD=/path/to/llvm-build ./run '^size\.large\.runtime\.closure$'
```

Runtime test names use
`run.<model>[.<platform>].<category>.<test>.<compiler>.<optimization>`;
the platform component is omitted for the default C167 simulator.

## Tests

`L`, `M`, `S`, `T` and `H` mean Large, Medium, Small, Tiny and Huge. `all`
means `O0,O1,O2,O3,Os,Oz`.

| Category | Test | Models | Optimizations | Variant / extra run |
|---|---|---:|---|---|
| ABI | `abi-aggregate-args` | L/M/S/T/H | all | shared source; aggregate arguments sized 1–8 bytes and Medium return instruction |
| ABI | `abi-aggregate-layout` | L/M/S/T/H | all | shared source; padded, packed, nested, union and bit-field aggregate layout and calls |
| ABI | `abi-aggregate-return` | L/M/S/T/H | all | shared source; direct/indirect 1–8-byte aggregate returns, callback state, tail stack arguments |
| ABI | `abi-byte-return` | L/M/S/T/H | all | shared source; signed byte argument/return plus Large/Medium instruction checks |
| ABI | `abi-code-bank` | L/S | all | LLVM-only standalone image; banked direct/indirect/nested calls, stack arguments, float/double values and `__banksw` state preservation |
| ABI | `abi-combined-matrix` | L/M/S/H | O0,O2,Oz | shared six-file program; mixed scalar/stack arguments, packed aggregate arguments, aggregate returns, data/function pointers, integer runtime helpers and float/double calls |
| ABI | `abi-far-automatic-matrix` | L/M/S/H | all | shared two-file source; explicit far pointers to large automatic objects through direct, nested and indirect calls with stack canaries |
| ABI | `abi-far-memory-matrix` | L/M/S/H | all | shared source; explicit far storage, loads/stores, mutation, arithmetic, relations, difference, null and exact 16 KiB page-boundary behavior |
| ABI | `abi-far-pointer` | L/M/S/T/H | all | shared source; default data-pointer argument, store and returned identity |
| ABI | `abi-generated-aggregate` | L/M/S/H | all | shared generated aggregate corpus with 32 shapes |
| ABI | `abi-global-data-lifecycle` | L/M/S/T/H | all | shared source; CRT `.data`/`.bss` initialization, `.rodata`, initialized pointers, mutations and persistent state |
| ABI | `abi-global-data-relocated` | L/M/S/H | all | LLVM-only reuse of `abi-global-data-lifecycle`; code, `.rodata`, `.data` and `.bss` relocation with exact symbol checks |
| ABI | `abi-global-function-pointer` | L/M/S/T/H | all | shared three-file source; global slot initialization, direct/indirect calls, identity, null handling and near/far lowering |
| ABI | `abi-linked-application` | L/M/S/T/H | O0,O2,Oz | LLVM-only standalone multi-object image; archive extraction, strong/weak selection, common coalescing, section GC, function table and callback |
| ABI | `abi-linked-application-relocated` | L/M/S/T/H | O0,O2,Oz | LLVM-only standalone image; the same archive/link/GC behavior at a second text/data placement |
| ABI | `abi-fp-calls` | L/M/S/H | all | shared source; mixed float/double register and stack arguments, returns, direct/indirect and nested calls, exact object bytes |
| ABI | `abi-huge-pointer` | L/M/H | all | shared source; huge/shuge access, arithmetic, comparison, page crossing, pointer casts and DPP-aware direct-address checks |
| ABI | `abi-indirect-call` | L/M/S/T/H | all | shared source; direct and indirect calls with register/stack arguments; near/far lowering checks |
| ABI | `abi-indirect-matrix` | L/M/S/H | all | shared source; volatile function-pointer storage, reload, return, identity, register/stack arguments and near/far indirect-call lowering |
| ABI | `abi-interrupt` | L/M/S/T/H | all | LLVM-only standalone image; real trap/vector/RETI path, nested ISR, architectural register preservation and exact ISR layout |
| ABI | `abi-long` | L/M/S/T/H | all | shared three-file program; 32-bit scalar and record arguments, globals and returns |
| ABI | `abi-medium-function-classes` | M | all | shared source; default near and explicit huge function calls, pointers and fixed-segment lowering |
| ABI | `abi-mixed-recursion` | L/M/S/T/H | all | shared source; two-TU mutual recursion through direct/indirect calls with scalar, float, aggregate, pointer, stack-tail and canary checks |
| ABI | `abi-mixed-slot-matrix` | L/M/S/H | all | shared three-file source; signed/unsigned byte, 32-bit, data/function-pointer and enum arguments at every register/stack slot |
| ABI | `abi-multi-epilog` | L/S | all | LLVM-only standalone stress-regalloc test; direct and state-probed early/loop exits with user/system-stack, callee-save and DPP preservation |
| ABI | `abi-near-function` | L/S | all | shared source; near/far function-pointer sizes, calls and fixed-segment lowering |
| ABI | `abi-near-pointer` | L/M | all | shared source; near/xnear access, arithmetic, comparison, casts, and null handling |
| ABI | `abi-nested-call-chains` | L/M/S/T/H | all | shared source; nested direct, indirect and reentrant calls with persistent state |
| ABI | `abi-pair-r12` | L/M/S/T/H | all | shared source; R12-aligned pair followed by register and stack arguments |
| ABI | `abi-pointer-casts` | L/S | all | shared source; data/function pointer-to-integer, integer-to-pointer and round-trip casts with DPP-aware Small expectations |
| ABI | `abi-qualified-globals` | L/M/S/H | all | shared source; initialized and zeroed near/xnear/far/huge/shuge globals |
| ABI | `abi-recursion-state` | L/M/S/T/H | all | shared five-file source; independent and cross-leaf recursion, forwarded calls, depth-8 frame canaries and fixed near/far routes |
| ABI | `abi-register-bank` | L/M/S/H | all | LLVM standalone; named-bank nested TRAP, priority IRQ, EXT/ATOMIC, PEC and interrupted-state preservation |
| ABI | `abi-register-pressure` | L/M/S/T/H | all | shared five-file source; word/long register and stack pressure, cross-selector leaves, forwarded calls, callee-save and exact near/far route checks |
| ABI | `abi-scalar` | L/M/S/T/H | all | shared source; mixed register/stack scalar arguments and 32-bit return |
| ABI | `abi-stack-page-limit` | L/M/S/T/H | all | shared three-file program; exact 16 KiB leaf frame and pointer into a large automatic object |
| ABI | `abi-stack-stop` | L/M/S/T/H | all | shared source; eight mixed register/stack argument vectors; TAP over ASC0 |
| ABI | `abi-stackparm` | L/M/S/H | all | shared source; mixed integer, far-pointer, aggregate, float and double stack parameters through direct, indirect and routed calls |
| ABI | `abi-tuple-reload` | L/M/S/H | all | LLVM-only standalone stress-regalloc test; direct and state-probed low/high reloads of six live 32-bit tuples with exact near/far call placement |
| ABI | `abi-varargs` | L/M/S/T/H | all | shared source; promoted signed byte, 16/32-bit values and pointer varargs; TAP over ASC0 |
| ABI | `abi-varargs-aggregate` | L/M/S/T/H | all | shared source; pair, byte aggregate, packed aggregate and stack-tail variadic arguments through direct and separate-TU caller paths |
| ABI | `abi-varargs-matrix` | L/M/S/H | all | shared source; five fixed-argument shapes, default promotions, 32-bit values, pointers, zero-to-seven-word stack prefixes, `va_copy`, and direct/separate-TU caller paths |
| ABI | `abi-word-matrix` | L/M/S/H | all | shared source; word arities 0–10 through direct, nested and typed-indirect calls, with exact Large banked and Medium near-call placement checks |
| ABI | `farptr` | L/M/S/T/H | all | shared source; basic pointer argument and volatile load/store |
| ABI | `regargs` | L/M/S/T/H | all | shared source; register arguments and scalar return |
| ABI | `reverse` | L/M/S/T/H | all | shared two-file caller/callee; five word arguments including the first stack slot |
| ABI | `saved` | L/M/S/T/H | all | direct LLVM assembly probe; R6–R9 preservation across a C call |
| ABI | `stack` | L/M/S/T/H | all | shared source; mixed register and stack arguments |
| ABI | `structret` | L/M/S/T/H | all | shared source; aggregate return |
| Codegen | `aggregate` | L/M/S/H | O2 | shared LLVM/TASKING source; direct TAP over ASC0 |
| Codegen | `alu-direct-memory` | L/M/S/T/H | O2 | C167/Super10 standalone LLVM ELF; 290816 ALU/compare-and-loop result/flags cases, GPR/SFR register/memory/immediates, word/byte indirect addressing and postincrement |
| Codegen | `c-assembly` | L/M/S/T/H | all | C167/Super10 standalone LLVM ELF; C↔assembly calls, preprocessor macros, local labels, data and BSS |
| Codegen | `backend-tuple-overlap` | L/S/H | O2 | direct LLVM MIR post-RA execution; overlapping pair copies and 16-to-32-bit extensions with exact layout, symbol and instruction-order checks |
| Codegen | `branch` | L/M/S/H | O2 | shared LLVM/TASKING source; direct TAP over ASC0 |
| Codegen | `code-segment-boundaries` | L/M/S/T/H | O2 | C167/Super10 standalone LLVM ELF; CALLR/JMPR/JB wrap, relaxation and CSP transitions across exact 64 KiB boundaries |
| Codegen | `computed-goto` | L/M | all | LLVM-only; direct TAP over ASC0; symbol and indirect-jump checks |
| Codegen | `conditional-transfers` | L/M/S/T/H | O2 | C167/Super10 direct LLVM images; 3072 condition/flags/stack cases across relative, absolute, indirect and call forms |
| Codegen | `dynamic-alloca` | L/M/S/T/H | all | LLVM-only; direct TAP over ASC0; builtin/aligned alloca, nested VLA lifetimes, fixed locals, callback stack arguments, early exits and repeated-call stack balance |
| Codegen | `extended-addressing` | L/M/S/T/H | O2 | C167/Super10 standalone LLVM ELF; 128 EXT read/store cases plus 64 nested EXTP/EXTS cases, counts 1..4, counter reload and expiry |
| Codegen | `esfr-addressing` | L/M/S/T/H | O2 | C167/Super10 standalone LLVM ELF; 68 SFR/ESFR routing/expiry cases plus 1280 EXT/ATOMIC flags cases; bit operations, MOV writes, PUSH reads, stack balance, counts 1..4 |
| Codegen | `external-branch-relaxation` | L/M/S/T/H | O2 | direct LLVM TAP plus host-side symbol/disassembly checks for external conditional `JMPR` relaxation to inverted short branch and `JMPS` |
| Codegen | `near-call-stack` | L/M/S/T/H | O2 | C167/Super10 direct LLVM images; 256 CALLR/PCALL/RETP return-address, stack-depth, saved-word and PSW cases |
| Codegen | `mov-memory` | L/M/S/T/H | O2 | C167/Super10 direct LLVM images; 5760 SFR/direct/indirect transfer/flags cases, byte predecrement, immediate and neighbor preservation |
| Codegen | `mov-indirect` | L/M/S/T/H | O2 | C167/Super10 direct LLVM images; 248064 word/byte load/store/copy cases, predecrement/postincrement, displacement, RL/RH registers, even/odd byte addresses, flags and source/neighbor preservation |
| Codegen | `sfr-bitfields` | L/M/S/H | all | LLVM-only; direct TAP over ASC0; SFR/ESFR word access, bit fields, attributed bit declarations and instruction-count checks |
| Codegen | `mov-extend` | L/M/S/T/H | O2 | C167/Super10 direct LLVM images; 81920 MOVBS/MOVBZ result/flags cases, all byte values, GPR/SFR/memory and source preservation |
| Codegen | `bit-writeback` | L/M/S/T/H | O2 | C167/Super10 direct LLVM images; 49152 JBC/JNBS cases, GPR/RAM boundaries, flags, short/long branches and neighbor preservation |
| Codegen | `bit-branch` | L/M/S/T/H | O2 | C167/Super10 direct LLVM images; 49152 JB/JNB cases, taken/not-taken, operand/flags preservation, GPR/RAM and short/long branches |
| Codegen | `bit-binary` | L/M/S/T/H | O2 | C167/Super10 direct LLVM images; 61440 BMOV/BMOVN/BAND/BOR/BXOR/BCMP result/flags cases, GPR/RAM aliasing, source and neighbor preservation |
| Codegen | `bit-update` | L/M/S/T/H | O2 | C167/Super10 direct LLVM images; 24576 BSET/BCLR result/flags cases, all bit indices, GPR/RAM boundaries and neighbor preservation |
| Codegen | `bit-field` | L/M/S/T/H | O2 | C167/Super10 direct LLVM images; 49152 BFLDL/BFLDH mask/data/result/flags cases, GPR/RAM boundaries, other-byte and neighbor preservation |
| Codegen | `trap` | L/M/S/T/H | O2 | C167/Super10 standalone LLVM ELF; 1536 segmented TRAP/RETI cases with native vector stubs, flags/priority, interrupt frame and CSP/SP restoration |
| Codegen | `protocol` | L/M/S/H | all | shared two-file source; mixed-width cross-translation-unit call; TAP over ASC0 |
| Codegen | `shift-rotate` | L/M/S/T/H | O2 | C167/Super10 direct LLVM images; 103680 SHL/SHR/ASHR/ROL/ROR result/flags cases, count masking, sign-fill and destination/count aliasing |
| Differential | `control-flow-matrix` | L/M/S/T/H | all | shared LLVM/TASKING source; CFG, switch and jump-table behavior with host-side indirect-jump and table-symbol checks |
| Differential | `byte-arithmetic` | L/M/S/T/H | all | shared two-file source; byte comparisons, wraparound arithmetic, unary and binary memory operations, and full-width immediates |
| Differential | `far-pointer-alias-matrix` | L/M/S/H | all | shared source; far-pointer relations, aliasing, overlap and copies |
| Differential | `float-varargs-matrix` | L/M/S/H | all | shared three-file source; direct/indirect variadic calls, float promotion, five fixed-argument shapes and exact binary32/binary64 words |
| Differential | `float-volatile-matrix` | L/M/S/T/H | all | shared source; 290 exact volatile binary32/binary64 load, store, copy, compound assignment and local operation checks; TAP over ASC0 |
| Differential | `float-width-matrix` | L/M/S/T/H | all | shared source; exact IEEE representation for both conversion directions |
| Differential | `ieee754-f32-{add,sub,mul,div}` | L/M/S/T/H | all | four shared-source programs; all 1209 compiler-rt IEEE-754 arithmetic vectors plus exact storage and call-path checks; TAP over ASC0 |
| Differential | `float32-conversion-matrix` | L/M/S/T/H | all | 305 defined integer conversions and all 196 ordered pairs of zero, subnormal, finite, infinity and NaN operands; 7 C predicates plus 3 LLVM helper checks; TAP over ASC0 |
| Differential | `float32-storage-matrix` | L/M/S/T/H | all | LLVM runtime: exact binary32 arrays, records, unions, initializers, copies and external storage; TAP over ASC0 |
| Differential | `ieee754-f64-{add,sub,mul,div}` | L/M/S/T/H | all | four shared-source programs; all 1387 compiler-rt IEEE-754 arithmetic vectors plus five regressions; LLVM also checks direct public helper entry points; TAP over ASC0 |
| Differential | `float64-conversion-matrix` | L/M/S/T/H | all | 130 defined integer conversions and all 196 ordered pairs of zero, subnormal, finite, infinity and NaN operands; 7 C predicates plus 3 LLVM helper checks; TAP over ASC0 |
| Differential | `float64-storage-matrix` | L/M/S/T/H | all | LLVM runtime: exact binary64 arrays, records, unions, initializers, copies and external storage; TAP over ASC0 |
| Differential | `float64-snapshot` | L/M/S/T/H | all | shared two-file source; opaque mutation and preservation of binary64 temporaries; TAP over ASC0 |
| Differential | `generated-type-semantics` | L/M/S/T/H | all | shared generated corpus; 864 aggregate, bit-field, packed, pointer, union, callback, recursion, CFG, integer and volatile checkpoints; TAP over ASC0 |
| Differential | `integer16-matrix` | L/M/S/T/H | all | ext |
| Differential | `integer32-matrix` | L/M/S/T/H | all | ext |
| Differential | `memory-builtins` | L/M/S/T/H | all | shared source; direct and indirect memcpy/memmove/memset calls, overlaps, zero length, return values and far-page buffers; TAP over ASC0 |
| Differential | `metamorphic-conversion-matrix` | L/M/S/T/H | all | shared source; 43 arithmetic identities, comparisons, branches, volatile operations and integer conversion checks; TAP over ASC0 |
| Differential | `pseudo-expansion-matrix` | L/M/S/H | all | shared source; near/far/huge byte accesses and instruction checks; TAP over ASC0 |
| Differential | `scalar-float-bytes` | L/M/S/T/H | all | shared two-file source; exact float/double words, object-byte access and widening; TAP over ASC0 |
| Differential | `stream-parser` | L/M/S/T/H | O0/O2/Oz | shared multi-file source; 20 streaming parser/ring-buffer cases, chunking, wrap, overflow, malformed and partial frames; TAP over ASC0 |
| Differential | `callback-state-machine` | L/M/S/T/H | O0/O2/Oz | shared multi-file source; 16 state-machine cases, indirect handlers, context callbacks, callback replacement and unsigned wrap; TAP over ASC0 |
| Differential | `record-codec` | L/M/S/T/H | O0/O2/Oz | shared source; 28 string/record scenarios, exact wire bytes, unaligned buffers, all short capacities/truncations and malformed input; TAP over ASC0 |
| Runtime | `atomic-runtime` | L/M/S/T/H | all | LLVM-only; direct TAP over ASC0; integer, floating, aggregate and locking atomics |
| Runtime | `backend-operation-matrix` | L/M/S/T/H | all | LLVM-only; direct TAP over ASC0; carry/borrow, signed shifts, widening multiply-add, div/rem and signed post-increment |
| Runtime | `clz32-matrix` | L/M/S/T/H | all | shared LLVM/TASKING source; TAP over ASC0; LLVM symbol and disassembly checks |
| Runtime | `bool-semantics` | L/M/S/T/H | all | shared LLVM/TASKING source; `_Bool` representation, calls, aggregate layout and varargs promotion; TAP over ASC0 |
| Runtime | `crt` | L/M/S/T/H | O2 | standalone LLVM startup and CRT versus TASKING cstart; ordinary and qualified `.data/.bss`; TAP over ASC0 |
| Runtime | `i64-arithmetic-matrix` | L/M/S/T/H | all | LLVM-only; direct TAP over ASC0; arithmetic, shifts, aggregate and variadic boundaries; runtime-symbol checks |
| Runtime | `i64-comparison-matrix` | L/M/S/T/H | all | LLVM-only; direct TAP over ASC0; unsigned/signed comparisons, shifts and conditional calls; symbol checks |
| Runtime | `i64-conversion-matrix` | L/M/S/T/H | all | LLVM-only; direct TAP over ASC0; signed/unsigned conversions between 64-bit integers and float/double; runtime-symbol checks |
| Runtime | `runtime` | L/M/S/T/H | O2 | LLVM-only; direct TAP over ASC0; compiler-runtime helpers and symbol checks |
| Runtime | `runtime-helper-matrix` | L/M/S/T/H | O2 | standalone LLVM; 110592 multiply, divide, remainder, shift and clz operations with 1728 checkpoints; TAP over ASC0 |
| Runtime | `float-helper-abi` | L/M/S/T/H | O2 | standalone LLVM; direct ABI calls to all near/far 64-bit load, store, and copy runtime helpers (near-only subset in Tiny); TAP over ASC0 plus symbol/disassembly checks |
| Runtime | `tasking-runtime-variants` | L/S | O2 | shared LLVM/TASKING source; integer, binary32, and binary64 runtime operations on C167/ext and Super10/ext2 |
| Stress | `generated-cfg-pressure` | L/S | O2 | LLVM-only standalone corpus; 2,048 exact CFG/register-pressure results with stressed greedy allocation, plus a Large run with the basic allocator |
| Stress | `seeded-abi-call` | L/S | all | shared source; 10,560 direct, separate-TU, and typed-indirect calls over 22 mixed ABI signatures with 1,320 golden checkpoints |
| Stress | `seeded-f32-differential` | L/M/S/H | all | shared source; 102400 generated binary32 arithmetic results with 1600 golden checkpoints |
| Stress | `seeded-f32-to-i32` | L/M/S/H | all | shared source; 204800 defined binary32-to-integer conversions with 800 golden checkpoints |
| Stress | `seeded-f64-differential` | L/M/S/H | all | shared LLVM/TASKING source; 128000 binary64 arithmetic/comparison results with 2000 checkpoints from an exact rational round-to-nearest-even oracle |
| Stress | `seeded-f64-to-f32` | L/M/S/H | all | shared source; 102400 strict binary64-to-binary32 conversions with 800 golden checkpoints |
| Stress | `seeded-far-pointer` | L | all | shared source; 16384 generated far-pointer operations with 2048 golden checkpoints |
| Stress | `seeded-integer-differential` | L/M/S/H | all | shared source; 103680 generated integer operations checked against 54 golden signatures |
| Size | `code-size-corpus` | L/M/S/T/H | LLVM Os / TASKING O2 | CMake-built 33-file source-object comparison; external runtimes excluded; shared 60-case runtime corpus |
| Size | `code-size-floating` | L/M/S/T/H | Os | shared 10-case float32/binary64 arithmetic, conversion and comparison runtime corpus |
| Size | `runtime-closure` | L/M/S/T/H | Os | linked ROM limits for individual helpers and the aggregate FP corpus |
