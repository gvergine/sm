#ifndef ENUMERATE_H
#define ENUMERATE_H

#include <stdio.h>

int enumerate_states(FILE* definition_file);
int enumerate_actions(FILE* definition_file);
int enumerate_events(FILE* definition_file);

#endif
