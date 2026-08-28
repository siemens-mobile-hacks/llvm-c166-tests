typedef signed char abi_s8;
typedef unsigned char abi_u8;
typedef unsigned int abi_u16;
typedef signed long abi_s32;
typedef unsigned long abi_u32;

typedef abi_u16 (*abi_function)(abi_u16 first, abi_u16 second);

struct __attribute__((packed)) abi_packed {
  abi_u8 first;
  abi_u16 word;
  abi_u8 last;
};

struct abi_result {
  abi_u8 first;
  abi_u16 word;
  abi_u8 last;
};

abi_u16 mixed_probe(abi_s8 signed_byte, abi_u8 unsigned_byte,
                    abi_u32 long_value, abi_u16 stack_word,
                    abi_u16 stack_tail);
abi_u16 aggregate_probe(abi_u16 prefix, struct abi_packed value,
                        abi_u16 tail);
struct abi_result make_result(abi_u16 seed);
extern volatile abi_u16 far_words[4];
abi_u16 pointer_target(abi_u16 first, abi_u16 second);
abi_u16 pointer_probe(abi_function function, volatile abi_u16 *address,
                      abi_u16 tail);
abi_u16 runtime_probe(abi_u32 first, abi_u32 second, abi_u16 shift);
abi_u16 float_probe(float first, float second, double third, double fourth,
                    abi_u16 tail);
