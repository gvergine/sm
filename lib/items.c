#include <stdlib.h>
#include <string.h>
#include "hashset.h"
#include "items.h"

int cmp_strings(const void* str1, const void* str2)
{
    return strcmp(str1, str2) == 0;    
}

int hash_string(const void* str)
{
    int k, sum = 0;
    for (k = 0; k < strlen(str); k++)
    {
        sum += ((unsigned char*)str)[k];
    }
    return sum % 256;
}


int cmp_states(const void* state1, const void* state2)
{
    if (state1 == state2) return 1;
    if (state1 == 0 || state2 == 0) return 0;    
    return cmp_strings(((state_t*)state1)->name,((state_t*)state2)->name);
}

int hash_state(const void* state)
{
    return hash_string(((state_t*)state)->name);
}

void del_state(void* state)
{
    state_t * s = (state_t*)state;
    free(s->name); // when i build a state name i must use strdup
    hashset_delete(s->enter_actions);
    hashset_delete(s->exit_actions);
    hashset_delete(s->events);
    free(state);
}


int cmp_events(const void* event1, const void* event2)
{
    if (event1 == event2) return 1;
    if (event1 == 0 || event2 == 0) return 0;    
    return cmp_strings(((event_t*)event1)->name,((event_t*)event2)->name);
}

int hash_event(const void* event)
{
    return hash_string(((event_t*)event)->name);
}

void del_event(void* event)
{
    event_t * e = (event_t*)event;
    free(e->name);
    hashset_delete(e->internal_actions);
    free(event);
}



