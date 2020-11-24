#include <sm/state_machine_validation.h>
#include "parsing.h"
#include "validate.h"

int validate(FILE* definition_file)
{
    state_machine_t * sm = state_machine_new(0);

    if(!parse(definition_file, sm))
    {
        state_machine_delete(sm);
        fclose(definition_file);
        return 0;
    }
    
    fclose(definition_file);

    list_t * issues_list;
    state_machine_validate(sm, &issues_list);

    int n_issues = list_count(issues_list);

    list_element_t * iterator = 0;
    while ((iterator = list_find_next_element(issues_list,iterator)) != 0)
    {
        sm_val_item_t* issue = (sm_val_item_t*)list_get_data(iterator);
        switch (issue->issue)
        {
            case SM_VAL_STATENOTREACHABLE:
            fprintf(stderr,"ERROR: State `%s` is not reachable.\n",issue->item);
            break;
            case SM_VAL_NOINITIALSTATE:
            fprintf(stderr,"ERROR: No state is defined as initial.\n");
            break;
        }
    }

    list_delete(issues_list);
    state_machine_delete(sm);
    return n_issues == 0;    
}
