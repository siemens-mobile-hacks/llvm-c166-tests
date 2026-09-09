	struct shape8 value;
	abi_u16 word0 = first + fifth;
	abi_u16 word1 = second + sixth;
	abi_u16 word2 = third + seventh;
	abi_u16 word3 = fourth + eighth;
	unsigned long expected_pair = ((unsigned long)word1 << 16) | word0;
	unsigned long pair;
	abi_u16 live_word;
	volatile abi_u8 *live_pointer;
	abi_u8 *output = value.bytes;
	struct shape3 callback_result;
	abi_u16 index;
	abi_u16 mismatch = 0;

	aggregate_live.pair = expected_pair;
	aggregate_live.word = word2;
	aggregate_live.bytes[0] = (abi_u8)(first ^ second);
	aggregate_live.bytes[1] = (abi_u8)((first ^ second) ^ 0xffU);
	aggregate_live.pointer = &aggregate_live.bytes[0];
	pair = aggregate_live.pair;
	live_word = aggregate_live.word;
	live_pointer = aggregate_live.pointer;
	callback_result = aggregate_callback(first);

	if (aggregate_live.pair != (expected_pair ^ 0xffffffffUL))
		mismatch = 1;
	if (aggregate_live.word != (abi_u16)(word2 ^ 0xffffU))
		mismatch = 1;
	if (live_word != word2)
		mismatch = 1;
	if (*live_pointer != (abi_u8)(first ^ second))
		mismatch = 1;

	for (index = 0; index < 3; ++index) {
		if (callback_result.bytes[index] != (abi_u8)(first + 0x33U + index * 0x23U))
			mismatch = 1;
	}
	output[0] = (abi_u8)pair;
	output[1] = (abi_u8)(pair >> 8);
	output[2] = (abi_u8)(pair >> 16);
	output[3] = (abi_u8)(pair >> 24);
	output[4] = (abi_u8)word2;
	output[5] = (abi_u8)(word2 >> 8);
	output[6] = (abi_u8)word3;
	output[7] = (abi_u8)(word3 >> 8);
	if (mismatch)
		output[0] ^= 0xffU;
	return value;
