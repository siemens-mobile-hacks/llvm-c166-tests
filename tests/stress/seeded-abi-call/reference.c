#include "functions.h"

#pragma fragment

#define ABI_MIX_DEFINE(prefix, id, name, parameters, body) \
  abi_u32 prefix##name parameters { body; }

ABI_MIXED_FUNCTION_LIST(ABI_MIX_DEFINE, tasking_)

abi_u32 tasking_enum5(enum abi_enum a, enum abi_enum b, enum abi_enum c,
                      enum abi_enum d, enum abi_enum e) {
  ABI_ENUM5_BODY(a, b, c, d, e);
}
