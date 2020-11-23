%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sm/state_machine_definition.h>
#include "parsing.h"

extern int yylex();

FILE *yyin; 

extern state_machine_t * ___sm;
extern char* ___current_event;
extern char* ___current_state;
extern list_t* ___transitions;

#define TARGET_ENTRY 0
#define TARGET_EXIT 1
#define TARGET_EVENT 2

int current_target;
int is_initial_set = 0;


%}

%union {
  char *sval;
}


%token INIT STATE EXIT ENTRY 
%token ON ACTION GOTO OPEN_BRAKET
%token CLOSE_BRAKET


%token <sval> ITEMNAME

%%
root:
	{
	    ___transitions = list_new(free_transition);
	}
	statedefinitions
	{
	    add_transitions(___sm);
	    cleanup_memory();
	}
	;

statedefinitions:
	statedefinitions statedefinition
	| statedefinition
	;

statedefinition:
	initialstate | normalstate
	;

initialstate:
	INIT STATE ITEMNAME
	{
	    state_machine_add_state(___sm,$3); 
	    state_machine_set_initial_state(___sm,$3);
	    free(___current_state);
	    ___current_state=strdup($3);
	    free($3);
	    if (is_initial_set)
	    {
	        yyerror("Initial state was already set!");
	    }
	    is_initial_set=1;
	} 
	OPEN_BRAKET eventhandlerdefinitions CLOSE_BRAKET  
	;

normalstate:
	STATE ITEMNAME
	{
	    state_machine_add_state(___sm,$2);
	    free(___current_state);
	    ___current_state=strdup($2);
	    free($2);
	}
	OPEN_BRAKET eventhandlerdefinitions CLOSE_BRAKET  
	;

eventhandlerdefinitions:
	eventhandlerdefinitions eventhandlerdefinition
	|
	;

eventhandlerdefinition:
	onentrydefinition | onexitdefinition | oneventdefinition
	;

onentrydefinition:
	ON ENTRY
	{
	    current_target = TARGET_ENTRY;
	}
	OPEN_BRAKET actionstatements CLOSE_BRAKET
	;

onexitdefinition:
	ON EXIT
	{
	    current_target = TARGET_EXIT;
	}
	OPEN_BRAKET actionstatements CLOSE_BRAKET
	;

oneventdefinition:
	ON ITEMNAME
	{
	    current_target = TARGET_EVENT;
	    free(___current_event);
	    ___current_event = strdup($2);
	    free($2);
	}
	OPEN_BRAKET statements CLOSE_BRAKET
	;

statements:
	statements statement
	| statement
	;

statement:
	actionstatement | transitionstatement
	;

actionstatements:
	actionstatements actionstatement
	|
	;

actionstatement:
	ACTION ITEMNAME
	{
	    if (current_target == TARGET_ENTRY)
	    {
	        state_machine_add_enter_action(___sm, ___current_state, $2);
	    }
	    else if (current_target == TARGET_EXIT)
	    {
	        state_machine_add_exit_action(___sm, ___current_state, $2);
	    }
	    else
	    {
	        state_machine_add_internal_action(___sm,
	                                          ___current_state,
	                                          ___current_event,
	                                          $2);
	    }
	    free($2);
	}
	;
	
transitionstatement:
	GOTO ITEMNAME
	{
	    transition_t * transition = malloc(sizeof(transition_t));
	    transition->from = strdup(___current_state);
	    transition->to = strdup($2);
	    transition->event = strdup(___current_event);
	    free($2);
	    list_insert(___transitions,transition);
	}
	;
%%











