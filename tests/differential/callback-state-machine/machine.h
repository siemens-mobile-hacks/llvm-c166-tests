#ifndef TEST_MACHINE_H
#define TEST_MACHINE_H

enum { IDLE_STATE, ACTIVE_STATE, ERROR_STATE };
enum { OPEN_EVENT, DATA_EVENT, CLOSE_EVENT, RESET_EVENT };
struct machine;
typedef unsigned long (*handler_fn)(struct machine *, unsigned int, unsigned long);
typedef void (*notify_fn)(void *, unsigned int, unsigned long);

struct machine {
  const handler_fn *handlers;
  notify_fn notify;
  void *context;
  unsigned long total;
  unsigned int state, seen, rejected;
};

unsigned long machine_dispatch(struct machine *, unsigned int, unsigned long);
unsigned long machine_total(const struct machine *);
unsigned long handle_idle(struct machine *, unsigned int, unsigned long);
unsigned long handle_active(struct machine *, unsigned int, unsigned long);
unsigned long handle_error(struct machine *, unsigned int, unsigned long);

#endif
