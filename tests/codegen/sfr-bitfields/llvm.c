#include "c166_test.h"

typedef unsigned int u16;

typedef union {
  u16 value;
  struct {
    unsigned bit0 : 1;
    unsigned rest : 15;
  } bits;
} register16;

#define CC1IC (*(volatile __sfr register16 *)0xff7aU)
#define T7IC (*(volatile __esfr register16 *)0xf17aU)

extern unsigned int cc1ic_bit1 __attribute__((c166_sfrbit(0xff7a, 1)));
extern unsigned int t7ic_bit1 __attribute__((c166_esfrbit(0xf17a, 1)));

C166_NOINLINE static void set_sfr_bit(void) {
  CC1IC.bits.bit0 = 1;
}

C166_NOINLINE static void clear_sfr_bit(void) {
  CC1IC.bits.bit0 = 0;
}

C166_NOINLINE static void write_sfr_bit(u16 value) {
  CC1IC.bits.bit0 = value;
}

C166_NOINLINE static void set_esfr_bit(void) {
  T7IC.bits.bit0 = 1;
}

C166_NOINLINE static void clear_esfr_bit(void) {
  T7IC.bits.bit0 = 0;
}

C166_NOINLINE static void write_esfr_bit(u16 value) {
  T7IC.bits.bit0 = value;
}

C166_NOINLINE static u16 read_sfr_declared_bit(void) {
  return cc1ic_bit1;
}

C166_NOINLINE static void set_sfr_declared_bit(void) {
  cc1ic_bit1 = 1;
}

C166_NOINLINE static void clear_sfr_declared_bit(void) {
  cc1ic_bit1 = 0;
}

C166_NOINLINE static void write_sfr_declared_bit(u16 value) {
  cc1ic_bit1 = value;
}

C166_NOINLINE static u16 read_esfr_declared_bit(void) {
  return t7ic_bit1;
}

C166_NOINLINE static void set_esfr_declared_bit(void) {
  t7ic_bit1 = 1;
}

C166_NOINLINE static void clear_esfr_declared_bit(void) {
  t7ic_bit1 = 0;
}

C166_NOINLINE static void write_esfr_declared_bit(u16 value) {
  t7ic_bit1 = value;
}

unsigned int llvm_entry(void) {
  u16 saved_sfr = CC1IC.value;
  u16 saved_esfr = T7IC.value;
  unsigned int result = 0;

  CC1IC.value = 0x002aU;
  set_sfr_bit();
  if (CC1IC.value != 0x002bU)
    result = 1;
  clear_sfr_bit();
  if (CC1IC.value != 0x002aU)
    result = 2;
  write_sfr_bit(3U);
  if (CC1IC.value != 0x002bU)
    result = 3;
  write_sfr_bit(2U);
  if (CC1IC.value != 0x002aU)
    result = 4;

  T7IC.value = 0x0014U;
  set_esfr_bit();
  if (T7IC.value != 0x0015U)
    result = 5;
  clear_esfr_bit();
  if (T7IC.value != 0x0014U)
    result = 6;
  write_esfr_bit(3U);
  if (T7IC.value != 0x0015U)
    result = 7;
  write_esfr_bit(2U);
  if (T7IC.value != 0x0014U)
    result = 8;

  CC1IC.value = 0x0029U;
  set_sfr_declared_bit();
  if (CC1IC.value != 0x002bU || read_sfr_declared_bit() != 1U)
    result = 9;
  clear_sfr_declared_bit();
  if (CC1IC.value != 0x0029U || read_sfr_declared_bit() != 0U)
    result = 10;
  write_sfr_declared_bit(2U);
  if (CC1IC.value != 0x002bU || read_sfr_declared_bit() != 1U)
    result = 11;
  write_sfr_declared_bit(0U);
  if (CC1IC.value != 0x0029U || read_sfr_declared_bit() != 0U)
    result = 12;

  T7IC.value = 0x0029U;
  set_esfr_declared_bit();
  if (T7IC.value != 0x002bU || read_esfr_declared_bit() != 1U)
    result = 13;
  clear_esfr_declared_bit();
  if (T7IC.value != 0x0029U || read_esfr_declared_bit() != 0U)
    result = 14;
  write_esfr_declared_bit(2U);
  if (T7IC.value != 0x002bU || read_esfr_declared_bit() != 1U)
    result = 15;
  write_esfr_declared_bit(0U);
  if (T7IC.value != 0x0029U || read_esfr_declared_bit() != 0U)
    result = 16;

  T7IC.value = saved_esfr;
  CC1IC.value = saved_sfr;
  return result;
}

void main(void) {
  tap_plan(1);
  tap_is_u32(llvm_entry(), 0U, "SFR and ESFR bit fields");
}
