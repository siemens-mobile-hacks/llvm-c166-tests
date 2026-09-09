RAW_ATTRIBUTE
abi_u32 RAW_FUNCTION(abi_u16 shape, double first, abi_u16 middle, double second) {
	switch (shape) {
		case 1:
			return TARGET(1)(0x1111U, first, middle, second);
		case 2:
			return TARGET(2)(0x1111U, 0x2222U, first, middle, second);
		case 3:
			return TARGET(3)(0x1111U, 0x2222U, 0x4444U, first, middle, second);
		case 4:
			return TARGET(4)(0x1111U, 0x2222U, 0x4444U, 0x8888U, first, middle, second);
		default:
			return TARGET(5)(0x1111U, 0x2222U, 0x4444U, 0x8888U, 0xabcdU, first, middle, second);
	}
}

/* The float expression goes through ellipsis: the caller must promote it. */
PROMOTE_ATTRIBUTE
abi_u32 PROMOTE_FUNCTION(abi_u16 shape, float value) {
	switch (shape) {
		case 1:
			return TARGET(1)(0x1111U, value, 0x55aaU, 1.0);
		case 2:
			return TARGET(2)(0x1111U, 0x2222U, value, 0x55aaU, 1.0);
		case 3:
			return TARGET(3)(0x1111U, 0x2222U, 0x4444U, value, 0x55aaU, 1.0);
		case 4:
			return TARGET(4)(0x1111U, 0x2222U, 0x4444U, 0x8888U, value, 0x55aaU, 1.0);
		default:
			return TARGET(5)(0x1111U, 0x2222U, 0x4444U, 0x8888U, 0xabcdU, value, 0x55aaU, 1.0);
	}
}
