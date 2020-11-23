#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sm/state_machine.h>
#include "items.h"
#include "hashset.h"

struct state_machine
{
    hashset_t* states; // set of state_t
    state_t * initial_state;
    state_t * current_state;
    pthread_mutex_t lock;
};

state_machine_t * state_machine_new(del_func delete)
{
    state_machine_t * sm = malloc(sizeof(state_machine_t));
    sm->states = hashset_new(cmp_states, hash_state, del_state, 256);
    sm->initial_state = 0;
    sm->current_state = 0;
    pthread_mutex_init(&sm->lock, 0);   
    return sm;
}

void state_machine_delete(state_machine_t * sm)
{
    hashset_delete(sm->states);
    pthread_mutex_destroy(&sm->lock);
    free(sm);
}

int state_machine_add_state(state_machine_t * sm, const char* state_name)
{
    state_t * new_state = malloc(sizeof(state_t));
    new_state->name = strdup(state_name);
    if (hashset_insert(sm->states,new_state))
    {
        new_state->enter_actions = hashset_new(cmp_strings, hash_string, free, 256);
        new_state->exit_actions = hashset_new(cmp_strings, hash_string, free, 256);
        new_state->events = hashset_new(cmp_events, hash_event, del_event, 256);
        return 1;
    }
    else
    {
        free(new_state->name);
        free(new_state);
        return 0;
    }
}

state_t* find_state_with_name(state_machine_t * sm, const char* name)
{
    state_t s;
    s.name = (char*)name;
    return (state_t *)hashset_get_data(hashset_find_element(sm->states,&s));
}

event_t* find_event_with_name(state_t * state, const char* name)
{
    event_t e;
    e.name = (char*)name;
    return (event_t *)hashset_get_data(hashset_find_element(state->events,&e));
}


int state_machine_set_transition(state_machine_t * sm, const char* from_state_name, const char* to_state_name, const char* event_name)
{
    // first of all, check if the two state exist    
    state_t * from_state = (state_t *)find_state_with_name(sm,from_state_name);
    state_t * to_state = (state_t *)find_state_with_name(sm,to_state_name);
    if (from_state == 0 || to_state == 0) return 0;
    
    // the two state exist, lets add the event
    event_t * event = find_event_with_name(from_state, event_name);
    if (event == 0)
    {
        // let's add it
        event = malloc(sizeof(event_t));
        event->name = strdup(event_name);
        event->internal_actions = hashset_new(cmp_strings, hash_string, free, 256);
        event->transition = to_state;
        hashset_insert(from_state->events, event);  
        return 1; 
    }    
    
    return 0;   
}

int state_machine_add_enter_action(state_machine_t * sm, const char* state_name, const char* enter_action_name)
{
    state_t * state = (state_t *)find_state_with_name(sm,state_name);
    if (state == 0) return 0;
    char * action_name_copy = strdup(enter_action_name);
    hashset_element_t * ret = hashset_insert(state->enter_actions, action_name_copy);
    if (ret == 0) free(action_name_copy);
    return ret != 0;    
}

int state_machine_add_exit_action(state_machine_t * sm, const char* state_name, const char* exit_action_name)
{
    state_t * state = (state_t *)find_state_with_name(sm,state_name);
    if (state == 0) return 0;
    char * action_name_copy = strdup(exit_action_name);
    hashset_element_t * ret = hashset_insert(state->exit_actions, action_name_copy);
    if (ret == 0) free(action_name_copy);
    return ret != 0;    
}

int state_machine_add_internal_action(state_machine_t * sm, const char* state_name, const char* event_name, const char* action_name)
{
    state_t * state = (state_t *)find_state_with_name(sm,state_name);
    if (state == 0) return 0;
    
    //lets add the event
    event_t * event = find_event_with_name(state, event_name);
    if (event == 0)
    {
        // let's add it
        event = malloc(sizeof(event_t));
        event->name = strdup(event_name);
        event->internal_actions = hashset_new(cmp_strings, hash_string, free, 256);
        hashset_insert(state->events, event);
        event->transition = 0;
    }
    
    return hashset_insert(event->internal_actions, strdup(action_name)) != 0;    
        
}

int state_machine_set_initial_state(state_machine_t * sm, const char* state_name)
{
    state_t * state = (state_t *)find_state_with_name(sm,state_name);
    if (state == 0) return 0;
    sm->initial_state = state;
    return 1;
}

list_t * set_to_list(hashset_t * set)
{
    list_t * l = list_new(0);
    hashset_element_t * iterator = 0;
    while((iterator = hashset_find_next_element(set,iterator)) != 0)
    {
        list_insert(l,hashset_get_data(iterator));
    }
    return l;
}


list_t * state_machine_reset(state_machine_t * sm)
{
    pthread_mutex_lock(&sm->lock);
    sm->current_state = sm->initial_state;
    list_t * ret = set_to_list(sm->current_state->enter_actions);
    pthread_mutex_unlock(&sm->lock);
    return ret;

}

list_t * state_machine_dispatch_event(state_machine_t * sm, const char* event_name)
{
    pthread_mutex_lock(&sm->lock);
    
    list_t * l = list_new(0);
    
    // first check if the event is at all handled in current state
    event_t* event = find_event_with_name(sm->current_state, event_name);
    if (event == 0) 
    {
        pthread_mutex_unlock(&sm->lock);
        return l; // the event is ignored
    }

    // then check if there is some internal action upon event on current state    
    list_t * internal_actions = set_to_list(event->internal_actions);
    list_append(l,internal_actions);
    list_delete(internal_actions);

    // if there will be a transition    
    if (event->transition)
    {
        // gather all exit actions
        list_t * exit_actions = set_to_list(sm->current_state->exit_actions);
        list_append(l,exit_actions);
        list_delete(exit_actions);
    
        // update current state
        sm->current_state = event->transition;
        
        // gather all enter actions
        list_t * enter_actions = set_to_list(sm->current_state->enter_actions);
        list_append(l,enter_actions);
        list_delete(enter_actions);

        // contestato: execute also all internal actions?
        // ---> check if the event is at all handled in current state
        
        /*
        event_t* event2 = find_event_with_name(sm->current_state, event_name);
        if (event != 0)
        {
            list_t * new_state_internal_actions = set_to_list(event2->internal_actions);
            list_append(l,new_state_internal_actions);
            list_delete(new_state_internal_actions);
        }
        */
           
    
    }
    
    

    pthread_mutex_unlock(&sm->lock);
        
    return l;
}

