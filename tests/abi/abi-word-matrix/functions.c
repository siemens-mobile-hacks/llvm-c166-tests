#include "functions.h"

#define WORD_ATTRIBUTE(number)                                                \
  C166_NOINLINE C166_SECTION(".abi.word" #number)

DEFINE_WORD_FUNCTIONS(words, WORD_ATTRIBUTE(0), WORD_ATTRIBUTE(1),
                      WORD_ATTRIBUTE(2), WORD_ATTRIBUTE(3), WORD_ATTRIBUTE(4),
                      WORD_ATTRIBUTE(5), WORD_ATTRIBUTE(6), WORD_ATTRIBUTE(7),
                      WORD_ATTRIBUTE(8), WORD_ATTRIBUTE(9), WORD_ATTRIBUTE(10))
