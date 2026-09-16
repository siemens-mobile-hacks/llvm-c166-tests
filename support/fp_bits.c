#include "c166_fp_bits.h"

float c166_f32_from_bits(unsigned long bits) {
  float value;
  unsigned char *bytes = (unsigned char *)&value;

  bytes[0] = (unsigned char)(bits >> 16);
  bytes[1] = (unsigned char)(bits >> 24);
  bytes[2] = (unsigned char)bits;
  bytes[3] = (unsigned char)(bits >> 8);
  return value;
}

unsigned long c166_f32_to_bits(float value) {
  const unsigned char *bytes = (const unsigned char *)&value;
  return ((unsigned long)bytes[1] << 24) | ((unsigned long)bytes[0] << 16) |
         ((unsigned long)bytes[3] << 8) | bytes[2];
}

unsigned long c166_f32_load_bits(const volatile float *value) {
  const volatile unsigned char *bytes = (const volatile unsigned char *)value;
  return ((unsigned long)bytes[1] << 24) | ((unsigned long)bytes[0] << 16) |
         ((unsigned long)bytes[3] << 8) | bytes[2];
}

void c166_f32_store_bits(volatile float *value, unsigned long bits) {
  volatile unsigned char *bytes = (volatile unsigned char *)value;

  bytes[0] = (unsigned char)(bits >> 16);
  bytes[1] = (unsigned char)(bits >> 24);
  bytes[2] = (unsigned char)bits;
  bytes[3] = (unsigned char)(bits >> 8);
}

double c166_f64_from_words(unsigned int w0, unsigned int w1, unsigned int w2,
                           unsigned int w3) {
  double value;
  unsigned char *bytes = (unsigned char *)&value;
  unsigned int words[4];
  unsigned int index;

  words[0] = w0;
  words[1] = w1;
  words[2] = w2;
  words[3] = w3;
  for (index = 0U; index != 4U; ++index) {
    bytes[index * 2U] = (unsigned char)words[index];
    bytes[index * 2U + 1U] = (unsigned char)(words[index] >> 8);
  }
  return value;
}

void c166_f64_to_words(double value, unsigned int *words) {
  const unsigned char *bytes = (const unsigned char *)&value;
  unsigned int index;

  for (index = 0U; index != 4U; ++index) {
    words[index] = (unsigned int)bytes[index * 2U] |
                   ((unsigned int)bytes[index * 2U + 1U] << 8);
  }
}

void c166_f64_load_words(const volatile double *value, unsigned int *words) {
  const volatile unsigned char *bytes = (const volatile unsigned char *)value;
  unsigned int index;

  for (index = 0U; index != 4U; ++index) {
    words[index] = (unsigned int)bytes[index * 2U] |
                   ((unsigned int)bytes[index * 2U + 1U] << 8);
  }
}

void c166_f64_store_words(volatile double *value, const unsigned int *words) {
  volatile unsigned char *bytes = (volatile unsigned char *)value;
  unsigned int index;

  for (index = 0U; index != 4U; ++index) {
    bytes[index * 2U] = (unsigned char)words[index];
    bytes[index * 2U + 1U] = (unsigned char)(words[index] >> 8);
  }
}
