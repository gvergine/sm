#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <sm/list.h>

typedef struct state_machine state_machine_t;

state_machine_t * state_machine_new(del_func delete);
void state_machine_delete(state_machine_t*);

int state_machine_add_state(state_machine_t * sm, const char* state_name);
int state_machine_set_transition(state_machine_t * sm, const char* from_state_name, const char* to_state_name, const char* event_name);
int state_machine_add_enter_action(state_machine_t * sm, const char* state_name, const char* enter_action_name);
int state_machine_add_exit_action(state_machine_t * sm, const char* state_name, const char* exit_action_name);
int state_machine_add_internal_action(state_machine_t * sm, const char* state_name, const char* event_name, const char* action_name);
int state_machine_set_initial_state(state_machine_t * sm, const char* state_name);


list_t * state_machine_reset(state_machine_t * sm);
list_t * state_machine_dispatch_event(state_machine_t * sm, const char* event_name);


#endif
