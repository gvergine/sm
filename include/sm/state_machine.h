#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <sm/list.h>

typedef struct state_machine state_machine_t;

list_t * state_machine_reset(state_machine_t * sm);
list_t * state_machine_dispatch_event(state_machine_t * sm, const char* event_name);

#endif
