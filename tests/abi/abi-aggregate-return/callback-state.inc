static void mutate_callback_state(void) {
	aggregate_live.pair ^= 0xffffffffUL;
	aggregate_live.word ^= 0xffffU;
	aggregate_live.pointer = &aggregate_live.bytes[1];
}
