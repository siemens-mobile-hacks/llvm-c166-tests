#include "c166-address-spaces.h"

typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef unsigned long abi_u32;

#if defined(__clang__)
#define ABI_STACKPARM __attribute__((c166_stackparm))
#else
#define ABI_STACKPARM _stackparm
#endif

struct abi_stackparm_pair {
  abi_u16 lo;
  abi_u16 hi;
};

typedef union {
  float value;
  abi_u16 words[2];
} abi_float_words;

typedef union {
  double value;
  abi_u16 words[4];
} abi_double_words;

typedef abi_u16 ABI_STACKPARM abi_stackparm_function(
    abi_u8 a, abi_u16 b, abi_u32 c, const abi_u16 _far *p,
    struct abi_stackparm_pair pair, abi_u16 d);
typedef float ABI_STACKPARM abi_stackparm_float_function(float value,
                                                         abi_u16 tail);
typedef double ABI_STACKPARM abi_stackparm_double_function(double value,
                                                           abi_u16 tail);
