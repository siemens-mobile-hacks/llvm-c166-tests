#ifndef C166_FP_BITS_H
#define C166_FP_BITS_H

float c166_f32_from_bits(unsigned long bits);
unsigned long c166_f32_to_bits(float value);
unsigned long c166_f32_load_bits(const volatile float *value);
void c166_f32_store_bits(volatile float *value, unsigned long bits);

double c166_f64_from_words(unsigned int w0, unsigned int w1, unsigned int w2,
                           unsigned int w3);
void c166_f64_to_words(double value, unsigned int *words);
void c166_f64_load_words(const volatile double *value, unsigned int *words);
void c166_f64_store_words(volatile double *value, const unsigned int *words);

#endif
