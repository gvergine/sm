#include "state_machine_priv.h"

int state_machine_list_events(state_machine_t* sm, list_t ** events_list)
{
    if (sm == 0 || events_list == 0) return -1;
    
    hashset_t * events_set = hashset_new(cmp_strings, hash_string, 0, 256);
    
    hashset_element_t * states_iterator = 0;
    
    while((states_iterator = hashset_find_next_element(sm->states,states_iterator)) != 0)
    {
        state_t * s = (state_t*)hashset_get_data(states_iterator);
        
        hashset_element_t * events_iterator = 0;
        while((events_iterator = hashset_find_next_element(s->events,events_iterator)) != 0)
        {
            event_t * e = (event_t*)hashset_get_data(events_iterator);
            hashset_insert(events_set,e->name);
        }
    }

    *events_list = set_to_list(events_set);
    hashset_delete(events_set);
    return 0;
}

int state_machine_list_actions(state_machine_t* sm, list_t ** actions_list)
{
    if (sm == 0 || actions_list == 0) return -1;
    
    hashset_t * actions_set = hashset_new(cmp_strings, hash_string, 0, 256);

    hashset_element_t * states_iterator = 0;
    while((states_iterator = hashset_find_next_element(sm->states,states_iterator)) != 0)
    {
        hashset_element_t * actions_iterator;

        state_t * s = (state_t*)hashset_get_data(states_iterator);

        hashset_element_t * events_iterator = 0;
        while((events_iterator = hashset_find_next_element(s->events,events_iterator)) != 0)
        {
            event_t * e = (event_t*)hashset_get_data(events_iterator);

            actions_iterator = 0;
            while((actions_iterator = hashset_find_next_element(e->internal_actions,actions_iterator)) != 0)
            {
                char * action = (char*)hashset_get_data(actions_iterator);
                hashset_insert(actions_set,action);
            }

        }

        actions_iterator = 0;
        while((actions_iterator = hashset_find_next_element(s->enter_actions,actions_iterator)) != 0)
        {
            char * action = (char*)hashset_get_data(actions_iterator);
            hashset_insert(actions_set,action);
        }

        actions_iterator = 0;
        while((actions_iterator = hashset_find_next_element(s->exit_actions,actions_iterator)) != 0)
        {
            char * action = (char*)hashset_get_data(actions_iterator);
            hashset_insert(actions_set,action);
        }

    }

    *actions_list = set_to_list(actions_set);
    hashset_delete(actions_set);
    return 0;
}

int state_machine_list_states(state_machine_t* sm, list_t ** states_list)
{
    if (sm == 0 || states_list == 0) return -1;
    
    hashset_t * states_set = hashset_new(cmp_strings, hash_string, 0, 256);
    
    hashset_element_t * states_iterator = 0;
    
    while((states_iterator = hashset_find_next_element(sm->states,states_iterator)) != 0)
    {
        state_t * s = (state_t*)hashset_get_data(states_iterator);
        hashset_insert(states_set,s->name);
    }
    *states_list = set_to_list(states_set);
    hashset_delete(states_set);
    return 0;
}
