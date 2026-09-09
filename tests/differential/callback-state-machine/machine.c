#include "machine.h"

static void transition(struct machine *m, unsigned int next) {
  m->state = next;
  m->notify(m->context, next, m->total);
}

static unsigned long result(const struct machine *m) {
  return m->total ^ ((unsigned long)m->state << 24) ^
         ((unsigned long)m->seen << 16);
}

unsigned long handle_idle(struct machine *m, unsigned int event, unsigned long value) {
  (void)value;
  if (event == OPEN_EVENT) {
    transition(m, ACTIVE_STATE);
  } else {
    ++m->rejected;
    transition(m, ERROR_STATE);
  }
  return result(m);
}

unsigned long handle_active(struct machine *m, unsigned int event, unsigned long value) {
  if (event == DATA_EVENT) {
    m->total += value;
  } else if (event == CLOSE_EVENT) {
    transition(m, IDLE_STATE);
  } else {
    ++m->rejected;
    transition(m, ERROR_STATE);
  }
  return result(m);
}

unsigned long handle_error(struct machine *m, unsigned int event, unsigned long value) {
  (void)value;
  if (event == RESET_EVENT)
    transition(m, IDLE_STATE);
  else
    ++m->rejected;
  return result(m);
}

unsigned long machine_dispatch(struct machine *m, unsigned int event, unsigned long value) {
  ++m->seen;
  return m->handlers[m->state](m, event, value);
}

unsigned long machine_total(const struct machine *m) {
  return m->total;
}
