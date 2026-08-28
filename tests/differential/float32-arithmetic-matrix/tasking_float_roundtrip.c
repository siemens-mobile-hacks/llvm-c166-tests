unsigned int tasking_float_observed_head;
unsigned int tasking_float_observed_tail;

float tasking_float_roundtrip(unsigned int head, float value,
                              unsigned int tail)
{
  tasking_float_observed_head = head;
  tasking_float_observed_tail = tail;
  return value;
}
