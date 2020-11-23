#ifndef STATE_MACHINE_PRIV_H
#define STATE_MACHINE_PRIV_H

#include <stdlib.h>
#include <pthread.h>
#include "items.h"
#include "hashset.h"

#include <sm/state_machine.h>
#include <sm/state_machine_definition.h>
#include <sm/state_machine_validation.h>

struct state_machine
{
    hashset_t* states;
    state_t * initial_state;
    state_t * current_state;
    pthread_mutex_t lock;
};

state_t* find_state_with_name(state_machine_t * sm, const char* name);

#endif
