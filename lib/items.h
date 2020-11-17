#ifndef ITEMS_H
#define ITEMS_H

#include <stdint.h>
#include "hashset.h"

int cmp_strings(const void* str1, const void* str2);

int hash_string(const void* str);


typedef struct state {
    char * name;
    hashset_t * enter_actions; // set di puntatori a hashset_element_t di action_names
    hashset_t * exit_actions; // set di puntatori a hashset_element_t di action_names
    hashset_t * events; // set di puntatori a event_t
} state_t;

int cmp_states(const void* state1, const void* state2);
int hash_state(const void* state);
void del_state(void* state);


typedef struct event {
    char * name;
    hashset_t * internal_actions; // set di puntatori a hashset_element_t action_names
    state_t * transition; // puntatore a state_t
} event_t;

int cmp_events(const void* event1, const void* event2);
int hash_event(const void* event);
void del_event(void* event);


#endif
