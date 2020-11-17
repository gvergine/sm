%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sm/state_machine.h>
extern int yylex();
extern int yyparse();
FILE *yyin; 
void yyerror(const char *s);
extern int line_num;
extern state_machine_t * sm;
void add_transitions();
void free_transition(void* t);
#define TARGET_ENTRY 0
#define TARGET_EXIT 1
#define TARGET_EVENT 2
int current_target;
char* current_event;
char* current_state;
int is_initial_set = 0;
typedef struct transition {
    char* from;
    char* to;
    char* event;
} transition_t;
list_t* transitions;
%}

%union {
  char *sval;
}

// define the constant-string tokens:
%token INIT STATE EXIT ENTRY 
%token ON ACTION GOTO OPEN_BRAKET
%token CLOSE_BRAKET

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <sval> ITEMNAME

%%
root:
	{transitions = list_new(free_transition);} statedefinitions { add_transitions(); }
	;

statedefinitions:
	statedefinitions statedefinition
	| statedefinition 	
	;

statedefinition:
        initialstate | normalstate
        ;

initialstate:
        INIT STATE ITEMNAME { 
        	state_machine_add_state(sm,$3); 
        	state_machine_set_initial_state(sm,$3);
        	free(current_state);
        	current_state=strdup($3);
        	free($3);
        	if (is_initial_set) { yyerror("Initial state was already set!"); }
        	is_initial_set=1;
        	
        } 
        OPEN_BRAKET eventhandlerdefinitions CLOSE_BRAKET  
        ;
normalstate:
        STATE ITEMNAME { 
        	state_machine_add_state(sm,$2);
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
	ON ENTRY { 
	    current_target = TARGET_ENTRY;
	} OPEN_BRAKET actionstatements CLOSE_BRAKET {  }
	;
	
onexitdefinition:
	ON EXIT { 
	    current_target = TARGET_EXIT;
	} 
	OPEN_BRAKET actionstatements CLOSE_BRAKET { }
	;
	
oneventdefinition:
	ON ITEMNAME { 
	    current_target = TARGET_EVENT;
	    free(current_event);
	    current_event = strdup($2);
	    free($2);
	} 
        OPEN_BRAKET statements CLOSE_BRAKET {  }
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
	ACTION ITEMNAME { 
		if (current_target == TARGET_ENTRY) {
		    state_machine_add_enter_action(sm, current_state, $2);
		} else if (current_target == TARGET_EXIT) {
		    state_machine_add_exit_action(sm, current_state, $2);
		} else { // event
		    state_machine_add_internal_action(sm, current_state, current_event, $2);
		} 
		free($2);
	}
	;
	
transitionstatement:
	GOTO ITEMNAME { 
	            transition_t * transition = malloc(sizeof(transition_t));
	            transition->from = strdup(current_state);
	            transition->to = strdup($2);
	            transition->event = strdup(current_event);
	            free($2);         
	            list_insert(transitions,transition);

	}
	;
%%


// TODO referenziare stati futuri.
// e.g. al momento state_machine_set_transition(sm, disabled, enabled_light_on, toggle);
// non funziona!

void add_transitions() {
    
    list_element_t * iterator = 0;
    
    while((iterator = list_find_next_element(transitions,iterator)) != 0) {
        transition_t * transition = (transition_t*)list_get_data(iterator);
	state_machine_set_transition(sm,transition->from, transition->to, transition->event);        
    }
    
    list_delete(transitions);
    
    free(current_state);
    free(current_event);
    

}

void free_transition(void* t) {
    transition_t * transition = (transition_t*)t;
    free(transition->from);
    free(transition->to);
    free(transition->event);
    free(transition);
}

void yyerror(const char *s)
{
    extern int line_num;
    extern char *yytext;

    fprintf(stderr,"Error: %s at symbol %s on line %d\n",s,yytext,line_num);
    exit(1);
}







