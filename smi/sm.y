%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sm/state_machine_definition.h>
#include "parsing.h"

extern int yylex();

FILE *yyin; 

extern state_machine_t * ___sm;

#define TARGET_ENTRY 0
#define TARGET_EXIT 1
#define TARGET_EVENT 2

int current_target;
char* current_event;
char* current_state;
int is_initial_set = 0;

list_t* transitions;
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
	    transitions = list_new(free_transition);
	}
	statedefinitions
	{
	    add_transitions(___sm, transitions);
	    free(current_state);
	    free(current_event);
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
	    free(current_state);
	    current_state=strdup($3);
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
	    free(current_state);
	    current_state=strdup($2);
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
	    free(current_event);
	    current_event = strdup($2);
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
	        state_machine_add_enter_action(___sm, current_state, $2);
	    }
	    else if (current_target == TARGET_EXIT)
	    {
	        state_machine_add_exit_action(___sm, current_state, $2);
	    }
	    else
	    {
	        state_machine_add_internal_action(___sm,
	                                          current_state,
	                                          current_event,
	                                          $2);
	    }
	    free($2);
	}
	;
	
transitionstatement:
	GOTO ITEMNAME
	{
	    transition_t * transition = malloc(sizeof(transition_t));
	    transition->from = strdup(current_state);
	    transition->to = strdup($2);
	    transition->event = strdup(current_event);
	    free($2);
	    list_insert(transitions,transition);
	}
	;
%%











