#include "enumerate.h"

#include <sm/state_machine_introspection.h>
#include "parsing.h"
#include "validate.h"

void print_strings(list_t * l)
{
    list_element_t * iterator = 0;
    while((iterator = list_find_next_element(l,iterator)) != 0)
    {
        char* s = (char*)list_get_data(iterator);
        fprintf(stdout,"%s\n",s);
        fflush(stdout);
    }
    list_delete(l);
}

int enumerate_states(FILE* definition_file)
{
    int ret;
    state_machine_t * sm = state_machine_new(0);

    if(!parse(definition_file, sm))
    {
        state_machine_delete(sm);
        fclose(definition_file);
        return 0;
    }

    fclose(definition_file);

    list_t * l;
    ret = state_machine_list_states(sm, &l);
    if(ret == 0) print_strings(l);
    state_machine_delete(sm);
    return ret == 0;
}

int enumerate_events(FILE* definition_file)
{
    int ret;
    state_machine_t * sm = state_machine_new(0);

    if(!parse(definition_file, sm))
    {
        state_machine_delete(sm);
        fclose(definition_file);
        return 0;
    }

    fclose(definition_file);

    list_t * l;
    ret = state_machine_list_events(sm, &l);
    if(ret == 0) print_strings(l);
    state_machine_delete(sm);
    return ret == 0;
}

int enumerate_actions(FILE* definition_file)
{
    int ret;
    state_machine_t * sm = state_machine_new(0);

    if(!parse(definition_file, sm))
    {
        state_machine_delete(sm);
        fclose(definition_file);
        return 0;
    }

    fclose(definition_file);

    list_t * l;
    ret = state_machine_list_actions(sm, &l);
    if(ret == 0) print_strings(l);
    state_machine_delete(sm);
    return ret == 0;
}
