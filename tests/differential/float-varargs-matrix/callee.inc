extern volatile double vararg_captured[2];
extern volatile abi_u16 vararg_observed[2];

static NOINLINE abi_u32 capture_varargs(va_list arguments, abi_u16 fixed_mix) {
	abi_u16 middle;
	vararg_captured[0] = va_arg(arguments, double);
	middle = va_arg(arguments, unsigned int);
	vararg_captured[1] = va_arg(arguments, double);
	vararg_observed[0] = middle;
	vararg_observed[1] = fixed_mix;
	return 0xa5a50000UL | fixed_mix;
}

CALLEE_ATTRIBUTE(1)
abi_u32 CALLEE(1)(abi_u16 fixed0, ...) {
	va_list arguments;
	abi_u32 result;
	va_start(arguments, fixed0);
	result = capture_varargs(arguments, fixed0);
	va_end(arguments);
	return result;
}

CALLEE_ATTRIBUTE(2)
abi_u32 CALLEE(2)(abi_u16 fixed0, abi_u16 fixed1, ...) {
	va_list arguments;
	abi_u32 result;
	va_start(arguments, fixed1);
	result = capture_varargs(arguments, fixed0 ^ fixed1);
	va_end(arguments);
	return result;
}

CALLEE_ATTRIBUTE(3)
abi_u32 CALLEE(3)(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2, ...) {
	va_list arguments;
	abi_u32 result;
	va_start(arguments, fixed2);
	result = capture_varargs(arguments, fixed0 ^ fixed1 ^ fixed2);
	va_end(arguments);
	return result;
}

CALLEE_ATTRIBUTE(4)
abi_u32 CALLEE(4)(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2, abi_u16 fixed3, ...) {
	va_list arguments;
	abi_u32 result;
	va_start(arguments, fixed3);
	result = capture_varargs(arguments, fixed0 ^ fixed1 ^ fixed2 ^ fixed3);
	va_end(arguments);
	return result;
}

CALLEE_ATTRIBUTE(5)
abi_u32 CALLEE(5)(abi_u16 fixed0, abi_u16 fixed1, abi_u16 fixed2, abi_u16 fixed3, abi_u16 fixed4, ...) {
	va_list arguments;
	abi_u32 result;
	va_start(arguments, fixed4);
	result = capture_varargs(arguments, fixed0 ^ fixed1 ^ fixed2 ^ fixed3 ^ fixed4);
	va_end(arguments);
	return result;
}
