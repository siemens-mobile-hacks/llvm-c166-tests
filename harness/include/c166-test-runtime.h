#ifndef C166_TEST_RUNTIME_H
#define C166_TEST_RUNTIME_H

/* Keep the debugger stop point in the host translation unit. */
void simulator_stop(void) {
  for (;;)
    ;
}

#endif
