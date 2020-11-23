#include <stdlib.h>
#include <stdio.h>
#include "sm.tab.h"
#include "parsing.h"

void free_transition(void* t)
{
    transition_t * transition = (transition_t*)t;
    free(transition->from);
    free(transition->to);
    free(transition->event);
    free(transition);
}

extern FILE * yyin;
extern void yylex_destroy();
state_machine_t * ___sm;
char* ___current_state;
char* ___current_event;
list_t* ___transitions;
int ret_parse = 1;

void add_transitions(state_machine_t* sm)
{
    list_element_t * iterator = 0;

    while((iterator = list_find_next_element(___transitions,iterator)) != 0)
    {
        transition_t * transition = (transition_t*)list_get_data(iterator);
        state_machine_set_transition(sm,transition->from, transition->to,
                                     transition->event);
    }

}






void cleanup_memory()
{
    list_delete(___transitions);
    free(___current_state);
    free(___current_event);
}

int parse(FILE* f, state_machine_t * sm)
{
    ___sm = sm;
    yyin = f;
    yyparse();
    yylex_destroy();
    return ret_parse;
}

void yyerror(const char *s)
{
    extern int line_num;
    extern char *yytext;
    fprintf(stderr,"Error: %s at symbol %s on line %d\n",s,yytext,line_num);
    yylex_destroy();
    cleanup_memory();
    ret_parse = 0;
}


