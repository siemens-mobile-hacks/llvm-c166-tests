#ifndef ABI_LINKED_APPLICATION_TYPES_H
#define ABI_LINKED_APPLICATION_TYPES_H

typedef unsigned int abi_u16;
typedef unsigned long abi_u32;
typedef abi_u16 (*linked_operation)(abi_u16);

#define LINKED_MARKER 0x4c31U

static abi_u16 linked_rotate_left(abi_u16 value, abi_u16 count) {
  return (abi_u16)((value << count) | (value >> (16U - count)));
}

static abi_u16 linked_archive_expected(abi_u16 value) {
  return (abi_u16)(linked_rotate_left(value, 3U) ^ 0x39a5U);
}

static abi_u16 linked_selected_expected(abi_u16 value) {
  return (abi_u16)(value + 0x2468U);
}

static abi_u16 linked_common_expected(abi_u16 state, abi_u16 value) {
  return (abi_u16)((state + value) ^ 0x0f0fU);
}

static abi_u16 linked_observe_expected(abi_u16 state, abi_u16 value) {
  return (abi_u16)(state ^ linked_rotate_left(value, 1U));
}

static abi_u16 linked_callback_expected(abi_u16 value, abi_u16 state) {
  return (abi_u16)((linked_rotate_left(value, 14U) + state) ^ 0xa55aU);
}

static abi_u16 linked_initial_state(abi_u16 seed) {
  return (abi_u16)(seed ^ 0x1357U);
}

#endif
