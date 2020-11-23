#ifndef STATE_MACHINE_VALIDATION_H
#define STATE_MACHINE_VALIDATION_H

#include <sm/state_machine_definition.h>

typedef enum sm_val_issue {
    SM_VAL_STATENOTREACHABLE = 0,
    SM_VAL_NOINITIALSTATE = 1
} sm_val_issue_e;

typedef struct sm_val_item {
    sm_val_issue_e issue;
    char* item;
} sm_val_item_t;

int state_machine_validate(state_machine_t * sm, list_t ** issues_list);

#endif
