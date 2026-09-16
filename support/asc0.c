typedef unsigned short u16;

#if defined(C166_TEST_TASKING)
#if C166_TEST_SUPER10
#include <regsuper10bo.h>
#else
#include <reg167.h>
#endif
#else
#define SFR16(address) (*(volatile __sfr u16 *)(address))
#define P3 SFR16(0xffc4U)
#define DP3 SFR16(0xffc6U)
#define S0TBUF SFR16(0xfeb0U)
#define S0BG SFR16(0xfeb4U)
#define S0TIC SFR16(0xff6cU)
#define S0RIC SFR16(0xff6eU)
#define S0EIC SFR16(0xff70U)
#define S0CON SFR16(0xffb0U)

extern unsigned int s0tir __attribute__((c166_sfrbit(0xff6c, 7)));
#define S0TIR_VALUE s0tir
#endif

#if defined(C166_TEST_TASKING)
#define S0TIR_VALUE S0TIR
#endif

void c166_asc0_init(void) {
  P3 |= 0x0400U;
  DP3 = (DP3 & 0xf3ffU) | 0x0400U;
  S0BG = 64U;
  S0TIC = 0;
  S0RIC = 0;
  S0EIC = 0;
  S0TIR_VALUE = 1;
  S0CON = 0x8011U;
}

void c166_asc0_putc(char value) {
  while (!S0TIR_VALUE)
    ;
  S0TIR_VALUE = 0;
  S0TBUF = (unsigned char)value;
}
