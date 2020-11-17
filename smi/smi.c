#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sm/state_machine.h>
#include "cmdline.h"
#include "sm.tab.h"

extern FILE * yyin;
extern void yylex_destroy();
state_machine_t * sm;


void print_actions(list_t * l, FILE* out) {
    list_element_t * iterator = 0;
    while((iterator = list_find_next_element(l,iterator)) != 0)
    {
        char* action = (char*)list_get_data(iterator);
        fprintf(out,"%s\n",action);
    }
    list_delete(l);
}

int main(int argc, char* argv[])
{
    struct gengetopt_args_info args_info;
    if (cmdline_parser (argc, argv, &args_info) != 0) return 1;
    
    FILE *definition_file = fopen(args_info.definition_arg, "r");
    FILE *input_file = fopen(args_info.input_arg, "r");
    FILE *output_file = fopen(args_info.output_arg, "w");
    
    cmdline_parser_free(&args_info);

    if (!definition_file)
    {
        fprintf(stderr,"Unable to open definition file %s\n",args_info.definition_arg);
        return -1;
    }
    if (!input_file)
    {
        fprintf(stderr,"Unable to open input file %s\n",args_info.input_arg);
        return -1;
    }
    if (!output_file)
    {
        fprintf(stderr,"Unable to open output file %s\n",args_info.output_arg);
        return -1;
    }
    
    yyin = definition_file;

    sm = state_machine_new(0);
     
    yyparse();
    yylex_destroy();
    fclose(definition_file);
    
    print_actions(state_machine_reset(sm), output_file);
    
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, input_file)) != -1) {
            line[read-1] = '\0';
            print_actions(state_machine_dispatch_event(sm,line), output_file);
    }
    
    if (line) free(line);
    
    state_machine_delete(sm);
    
    fclose(input_file);
    fclose(output_file);
    return 0;
}
