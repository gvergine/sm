#include "state_machine_priv.h"
#include <stdio.h>

void free_val_item(void* item)
{
    free(item);
}

void find_reachable_states(hashset_t* reachable_states, state_t* initial)
{

    hashset_element_t * iterator = 0;
    while((iterator = hashset_find_next_element(initial->events,iterator)) != 0)
    {
        event_t * e = (event_t*)hashset_get_data(iterator);
        if (e->transition == 0) continue;

        if(hashset_insert(reachable_states,e->transition))
        {
            find_reachable_states(reachable_states, e->transition);
        }
    }
}

void add_issue(list_t* issues_list, sm_val_issue_e issue, char* item)
{
    sm_val_item_t * val_item = malloc(sizeof(sm_val_item_t));
    val_item->issue = issue;
    val_item->item = item;   
    list_insert(issues_list,val_item);
}

int state_machine_validate(state_machine_t * sm, list_t ** issues_list)
{
    *issues_list = list_new(free_val_item);
    
    if (sm->initial_state == 0)
    {
        add_issue(*issues_list, SM_VAL_NOINITIALSTATE, 0);
    }
    
    // if no initial state, no state is reachable
    if (sm->initial_state == 0)
    {
        hashset_element_t * iterator = 0;
        while((iterator = hashset_find_next_element(sm->states,iterator)) != 0)
        {
            state_t * s = (state_t*)hashset_get_data(iterator);
            add_issue(*issues_list, SM_VAL_STATENOTREACHABLE, s->name);
        }
    }
    else
    {
        hashset_t * reachable_states = hashset_new(cmp_states, hash_state, 0,
                                                   256);
        hashset_insert(reachable_states, sm->initial_state);
        
        find_reachable_states(reachable_states, sm->initial_state);
        hashset_element_t * iterator = 0;
        
        while((iterator = hashset_find_next_element(sm->states,iterator)) != 0)
        {
            state_t * s = (state_t*)hashset_get_data(iterator);

            if (!hashset_contains(reachable_states,s))
            {
                add_issue(*issues_list, SM_VAL_STATENOTREACHABLE, s->name);
            }            
        }        
        
        hashset_delete(reachable_states);
    }
    
    return 0;
}
