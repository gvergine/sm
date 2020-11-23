#ifndef PARSING_H
#define PARSING_H

#include <sm/state_machine_validation.h>

typedef struct transition {
    char* from;
    char* to;
    char* event;
} transition_t;

void add_transitions(state_machine_t* sm);
void free_transition(void* t);

void yyerror(const char *s);
void cleanup_memory();
int parse(FILE* f, state_machine_t * sm);

#endif
