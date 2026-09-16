#include "c166_test.h"

extern unsigned int c166_five(unsigned int a, unsigned int b, unsigned int c,
                              unsigned int d, unsigned int e);

C166_NOINLINE
unsigned int c166_reverse_entry(unsigned int a, unsigned int b, unsigned int c,
                                unsigned int d, unsigned int e) {
  return c166_five(a, b, c, d, e);
}

struct vector {
  unsigned int a;
  unsigned int b;
  unsigned int c;
  unsigned int d;
  unsigned int e;
  unsigned int expected;
};

static const struct vector vectors[] = {
    {1U, 2U, 3U, 4U, 5U, 55U},          {0xffffU, 0U, 0U, 0U, 0U, 0xffffU},
    {0U, 0xffffU, 0U, 0U, 0U, 0xfffeU}, {0U, 0U, 0xffffU, 0U, 0U, 0xfffdU},
    {0U, 0U, 0U, 0xffffU, 0U, 0xfffcU}, {0U, 0U, 0U, 0U, 0xffffU, 0xfffbU},
};

void main(void) {
  unsigned int i;

  tap_plan(sizeof(vectors) / sizeof(vectors[0]));
  for (i = 0; i != sizeof(vectors) / sizeof(vectors[0]); ++i) {
    const struct vector *vector = &vectors[i];
    tap_is_u32(c166_reverse_entry(vector->a, vector->b, vector->c, vector->d,
                                  vector->e),
               vector->expected, "five word arguments");
  }
}
