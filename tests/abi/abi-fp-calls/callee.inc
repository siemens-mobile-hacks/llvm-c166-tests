/* Observe the parameter objects and return the original FP value unchanged. */
FLOAT_ATTRIBUTE
float FLOAT_FUNCTION(abi_u16 tag, double first, float value, abi_u16 tail, abi_u8 *out) {
	abi_u16 i;
	for (i = 0; i != 8; ++i)
		out[i] = ((abi_u8 *)&first)[i];
	for (i = 0; i != 4; ++i)
		out[8 + i] = ((abi_u8 *)&value)[i];
	out[12] = (abi_u8)tag;
	out[13] = (abi_u8)(tag >> 8);
	out[14] = (abi_u8)tail;
	out[15] = (abi_u8)(tail >> 8);
	return value;
}

DOUBLE_ATTRIBUTE
double DOUBLE_FUNCTION(abi_u16 tag, float first, double value, abi_u16 tail, abi_u8 *out) {
	abi_u16 i;
	for (i = 0; i != 4; ++i)
		out[i] = ((abi_u8 *)&first)[i];
	for (i = 0; i != 8; ++i)
		out[4 + i] = ((abi_u8 *)&value)[i];
	out[12] = (abi_u8)tag;
	out[13] = (abi_u8)(tag >> 8);
	out[14] = (abi_u8)tail;
	out[15] = (abi_u8)(tail >> 8);
	return value;
}
