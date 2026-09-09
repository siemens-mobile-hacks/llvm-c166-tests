#include "machine.h"

struct event { unsigned int kind; unsigned long value; };
static const handler_fn handlers[] = {handle_idle, handle_active, handle_error};
static const struct event events[4][7] = {
  {{OPEN_EVENT,0}, {DATA_EVENT,5}, {DATA_EVENT,7}, {CLOSE_EVENT,0}},
  {{DATA_EVENT,9}, {OPEN_EVENT,0}, {RESET_EVENT,0}, {OPEN_EVENT,0},
   {DATA_EVENT,32}, {CLOSE_EVENT,0}},
  {{OPEN_EVENT,0}, {OPEN_EVENT,0}, {DATA_EVENT,9}, {RESET_EVENT,0},
   {CLOSE_EVENT,0}, {RESET_EVENT,0}},
  {{OPEN_EVENT,0}, {DATA_EVENT,0}, {CLOSE_EVENT,0}, {OPEN_EVENT,0},
   {DATA_EVENT,0x12345678UL}, {DATA_EVENT,0xffffffffUL}, {CLOSE_EVENT,0}}
};
static const unsigned int lengths[] = {4,6,6,7};
static const unsigned int rejected[] = {0,2,3,0};
static const unsigned int notifications[] = {2,4,5,4};
/* Notification state sequences, encoded by trace=trace*4+(state+1):
 * [active,idle], [error,idle,active,idle],
 * [active,error,idle,error,idle], [active,idle,active,idle]. */
static const unsigned int traces[] = {9,217,733,153};
static const unsigned long deltas[] = {12,32,0,0x12345677UL};

struct observer {
  struct machine *owner;
  unsigned int calls, alternate_calls, trace, bad, replace;
  unsigned long last_total;
};

static void observe(struct observer *o, unsigned int state, unsigned long total) {
  ++o->calls;
  o->trace = o->trace * 4U + state + 1U;
  o->last_total = total;
  /* Re-enter another translation unit while the dispatcher is on the stack. */
  if (machine_total(o->owner) != total || o->owner->state != state) o->bad = 1;
}

static void notify_alternate(void *context, unsigned int state, unsigned long total) {
  struct observer *o = context;
  ++o->alternate_calls;
  observe(o, state, total);
}

static void notify_initial(void *context, unsigned int state, unsigned long total) {
  struct observer *o = context;
  observe(o, state, total);
  if (o->replace) o->owner->notify = notify_alternate;
}

unsigned long c166_test_case(unsigned int case_id) {
  struct {
    unsigned int before;
    struct machine m;
    unsigned int after;
  } guarded;
  struct observer o;
  struct machine *m = &guarded.m;
  unsigned int path = case_id & 3U, i;
  unsigned long seed = (case_id & 4U) ? 0xfffffff0UL : 0;
  unsigned long expected = seed + deltas[path];
  unsigned long returned = 0, failure = 0;

  guarded.before = 0xa55a;
  guarded.after = 0x5aa5;
  m->handlers = handlers;
  m->notify = notify_initial;
  m->context = &o;
  m->total = seed;
  m->state = m->seen = m->rejected = 0;
  o.owner = m;
  o.calls = o.alternate_calls = o.trace = o.bad = 0;
  o.replace = (case_id & 8U) != 0;
  o.last_total = 0;
  for (i = 0; i != lengths[path]; ++i)
    returned = machine_dispatch(m, events[path][i].kind, events[path][i].value);

  if (m->state != IDLE_STATE || m->seen != lengths[path] || m->rejected != rejected[path])
    failure |= 2UL;
  if (m->total != expected || returned != (expected ^ ((unsigned long)lengths[path] << 16)))
    failure |= 4UL;
  if (o.calls != notifications[path] || o.trace != traces[path] || o.bad || o.last_total != expected)
    failure |= 8UL;
  if (o.alternate_calls != (o.replace ? notifications[path] - 1U : 0U))
    failure |= 16UL;
  if (m->notify != (o.replace ? notify_alternate : notify_initial) || m->context != &o)
    failure |= 32UL;
  if (guarded.before != 0xa55a || guarded.after != 0x5aa5) failure |= 64UL;
  return 1UL | failure;
}
