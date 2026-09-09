typedef struct shape3 (*abi_return3_fn)(abi_u16);
typedef struct shape8 (*abi_return_tail_fn)(abi_u16, abi_u16, abi_u16, abi_u16,
		abi_u16, abi_u16, abi_u16, abi_u16);

/* Compare only initialized bytes; shape3 has one byte of padding. */
static abi_u16 check_indirect_returns(abi_u16 seed, const struct shape3 *expected3,
		const struct shape8 *expected_tail, abi_return3_fn return3, abi_return_tail_fn return_tail) {
	abi_return3_fn volatile call3 = return3;
	abi_return_tail_fn volatile call_tail = return_tail;
	volatile struct {
		abi_u16 before;
		struct shape3 first;
		abi_u16 between;
		struct shape8 tail;
		abi_u16 after;
	} storage;
	abi_u16 index;

	storage.before = seed ^ 0xa55aU;
	storage.between = seed ^ 0x369cU;
	storage.after = seed ^ 0x5aa5U;
	storage.first = call3(seed);
	storage.tail = call_tail(TAIL_ARGUMENTS(seed));

	if (storage.before != (abi_u16)(seed ^ 0xa55aU))
		return 0;
	if (storage.between != (abi_u16)(seed ^ 0x369cU))
		return 0;
	if (storage.after != (abi_u16)(seed ^ 0x5aa5U))
		return 0;
	for (index = 0; index < 3; ++index) {
		if (storage.first.bytes[index] != expected3->bytes[index])
			return 0;
	}
	for (index = 0; index < 8; ++index) {
		if (storage.tail.bytes[index] != expected_tail->bytes[index])
			return 0;
	}
	return 1;
}
