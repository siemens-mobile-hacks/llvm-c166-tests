#ifndef ABI_AGGREGATE_RETURN_TYPES_H
#define ABI_AGGREGATE_RETURN_TYPES_H

typedef unsigned char abi_u8;
typedef unsigned int abi_u16;

struct shape1 { abi_u8 bytes[1]; };
struct shape2 { abi_u8 bytes[2]; };
struct shape3 { abi_u8 bytes[3]; };
struct shape4 { abi_u8 bytes[4]; };
struct shape5 { abi_u8 bytes[5]; };
struct shape6 { abi_u8 bytes[6]; };
struct shape7 { abi_u8 bytes[7]; };
struct shape8 { abi_u8 bytes[8]; };

typedef char assert_shape1_size[sizeof(struct shape1) == 2 ? 1 : -1];
typedef char assert_shape2_size[sizeof(struct shape2) == 2 ? 1 : -1];
typedef char assert_shape3_size[sizeof(struct shape3) == 4 ? 1 : -1];
typedef char assert_shape4_size[sizeof(struct shape4) == 4 ? 1 : -1];
typedef char assert_shape5_size[sizeof(struct shape5) == 6 ? 1 : -1];
typedef char assert_shape6_size[sizeof(struct shape6) == 6 ? 1 : -1];
typedef char assert_shape7_size[sizeof(struct shape7) == 8 ? 1 : -1];
typedef char assert_shape8_size[sizeof(struct shape8) == 8 ? 1 : -1];

#define DECLARE_RETURN(prefix, size) \
  struct shape##size prefix##size(abi_u16 seed)

DECLARE_RETURN(tasking_return, 1);
DECLARE_RETURN(tasking_return, 2);
DECLARE_RETURN(tasking_return, 3);
DECLARE_RETURN(tasking_return, 4);
DECLARE_RETURN(tasking_return, 5);
DECLARE_RETURN(tasking_return, 6);
DECLARE_RETURN(tasking_return, 7);
DECLARE_RETURN(tasking_return, 8);

DECLARE_RETURN(llvm_return, 1);
DECLARE_RETURN(llvm_return, 2);
DECLARE_RETURN(llvm_return, 3);
DECLARE_RETURN(llvm_return, 4);
DECLARE_RETURN(llvm_return, 5);
DECLARE_RETURN(llvm_return, 6);
DECLARE_RETURN(llvm_return, 7);
DECLARE_RETURN(llvm_return, 8);

#define DECLARE_TAIL_RETURN(prefix) \
  struct shape8 prefix(abi_u16 first, abi_u16 second, abi_u16 third, \
                       abi_u16 fourth, abi_u16 fifth, abi_u16 sixth, \
                       abi_u16 seventh, abi_u16 eighth)

DECLARE_TAIL_RETURN(tasking_return_tail);
DECLARE_TAIL_RETURN(llvm_return_tail);

#endif
