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

void add_transitions(state_machine_t* sm, list_t* transitions)
{
    list_element_t * iterator = 0;

    while((iterator = list_find_next_element(transitions,iterator)) != 0)
    {
        transition_t * transition = (transition_t*)list_get_data(iterator);
        state_machine_set_transition(sm,transition->from, transition->to,
                                     transition->event);
    }

    list_delete(transitions);

}


void yyerror(const char *s)
{
    extern int line_num;
    extern char *yytext;
    fprintf(stderr,"Error: %s at symbol %s on line %d\n",s,yytext,line_num);
    exit(1);
}

extern FILE * yyin;
extern void yylex_destroy();
state_machine_t * ___sm;
void parse(FILE* f, state_machine_t * sm)
{
    ___sm = sm;
    yyin = f;
    yyparse();
    yylex_destroy();
}


