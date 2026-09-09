typedef unsigned int u16;
typedef unsigned char u8;

u16 read_first_word(const void *object) {
  const u8 *bytes = (const u8 *)object;
  return (u16)bytes[0] | ((u16)bytes[1] << 8);
}

u16 double_parameter_word(double value) {
  return read_first_word(&value);
}

u16 extended_parameter_word(float value) {
	double extended = value;
	return read_first_word(&extended);
}

void store_two(void *object, u16 size) {
  u8 *bytes = (u8 *)object;
  u16 index;
  for (index = 0; index < size; ++index)
    bytes[index] = 0;
  bytes[1] = 0x40;
}
