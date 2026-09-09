#include "c166-test-runtime.h"
#include "c166-test-result.h"
#include "types.h"
#include "vectors.inc"

extern unsigned long aggregate_state_call(abi_u16 seed, abi_u16 route);

static void run_return_vector(abi_u16 vector_id, abi_u16 seed,
		unsigned long golden) {
	c166_test_check_u32(vector_id * 3 - 2, golden, aggregate_state_call(seed, 0));
	c166_test_check_u32(vector_id * 3 - 1, golden, aggregate_state_call(seed, 1));
	c166_test_check_u32(vector_id * 3, golden, aggregate_state_call(seed, 2));
}

#define RUN_RETURN(id, seed, golden) run_return_vector(id, seed, golden);

void main(void) {
	c166_test_begin(266, 0x1660010aUL);
	ABI_AGGREGATE_RETURN_VECTORS(RUN_RETURN)
	c166_test_finish();
	simulator_stop();
}
