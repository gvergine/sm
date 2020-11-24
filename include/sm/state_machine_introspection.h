#ifndef STATE_MACHINE_INTROSPECTION_H
#define STATE_MACHINE_INTROSPECTION_H

#include <sm/state_machine.h>

int state_machine_list_events(state_machine_t*, list_t ** events_list);
int state_machine_list_actions(state_machine_t*, list_t ** actions_list);
int state_machine_list_states(state_machine_t*, list_t ** states_list);

#endif
