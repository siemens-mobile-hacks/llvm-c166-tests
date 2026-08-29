#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "vectors.inc"

extern signed char llvm_entry_proxy(signed char value);
extern signed char c166_test_entry(signed char value);

static void run_byte_vector(unsigned int vector_id, signed char value,
                            unsigned long golden) {
  signed char reference = c166_test_entry(value);
  signed char actual = llvm_entry_proxy(value);
  c166_test_check_u32(vector_id * 2 - 1, golden,
                      (unsigned long)(unsigned char)reference);
  c166_test_check_u32(vector_id * 2, golden,
                      (unsigned long)(unsigned char)actual);
}

#define RUN_BYTE(id, value, golden) run_byte_vector(id, value, golden);

void main(void) {
  c166_test_begin(259, 0x16600103UL);
  ABI_BYTE_RETURN_VECTORS(RUN_BYTE)
  c166_test_finish();
  simulator_stop();
}
