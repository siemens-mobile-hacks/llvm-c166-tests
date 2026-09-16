#include "functions.h"

#define ABI_STRING_1(value) #value
#define ABI_STRING(value) ABI_STRING_1(value)
#define ABI_MIX_DEFINE(prefix, id, name, parameters, body)                  \
  C166_NOINLINE C166_SECTION(".abi_mixed" ABI_STRING(id))                 \
  abi_u32 prefix##name parameters { body; }

ABI_MIXED_FUNCTION_LIST(ABI_MIX_DEFINE, mixed_)

C166_NOINLINE C166_SECTION(".abi_mixed_enum5")
abi_u32 mixed_enum5(enum abi_enum a, enum abi_enum b, enum abi_enum c,
                    enum abi_enum d, enum abi_enum e) {
  ABI_ENUM5_BODY(a, b, c, d, e);
}
