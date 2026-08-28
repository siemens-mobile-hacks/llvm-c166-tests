#ifndef ABI_WORD_MATRIX_TYPES_H
#define ABI_WORD_MATRIX_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

#define DECLARE_WORD_FUNCTIONS(prefix) \
  abi_u32 prefix##0(void); \
  abi_u32 prefix##1(abi_u16 a0); \
  abi_u32 prefix##2(abi_u16 a0, abi_u16 a1); \
  abi_u32 prefix##3(abi_u16 a0, abi_u16 a1, abi_u16 a2); \
  abi_u32 prefix##4(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3); \
  abi_u32 prefix##5(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                     abi_u16 a4); \
  abi_u32 prefix##6(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                     abi_u16 a4, abi_u16 a5); \
  abi_u32 prefix##7(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                     abi_u16 a4, abi_u16 a5, abi_u16 a6); \
  abi_u32 prefix##8(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                     abi_u16 a4, abi_u16 a5, abi_u16 a6, abi_u16 a7); \
  abi_u32 prefix##9(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                     abi_u16 a4, abi_u16 a5, abi_u16 a6, abi_u16 a7, \
                     abi_u16 a8); \
  abi_u32 prefix##10(abi_u16 a0, abi_u16 a1, abi_u16 a2, abi_u16 a3, \
                      abi_u16 a4, abi_u16 a5, abi_u16 a6, abi_u16 a7, \
                      abi_u16 a8, abi_u16 a9)

DECLARE_WORD_FUNCTIONS(tasking_words);
DECLARE_WORD_FUNCTIONS(llvm_words);
DECLARE_WORD_FUNCTIONS(llvm_words_proxy);

abi_u32 llvm_reverse_words(abi_u16 shape, abi_u16 a0, abi_u16 a1,
                           abi_u16 a2, abi_u16 a3, abi_u16 a4, abi_u16 a5,
                           abi_u16 a6, abi_u16 a7, abi_u16 a8, abi_u16 a9);

#endif
