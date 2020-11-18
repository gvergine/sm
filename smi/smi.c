#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sm/state_machine.h>
#include "cmdline.h"
#include "sm.tab.h"

extern FILE * yyin;
extern void yylex_destroy();
state_machine_t * sm;


void print_actions(list_t * l, FILE* out, char* context) {
    int lines = 0;
    list_element_t * iterator = 0;
    while((iterator = list_find_next_element(l,iterator)) != 0)
    {
        char* action = (char*)list_get_data(iterator);
        fprintf(out,"%s %s\n",action,context);
        lines++;
    }
    list_delete(l);
    
    if(lines == 0) // event produced no action
    {
        fprintf(out,"- %s\n",context);
    }
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
    
    print_actions(state_machine_reset(sm), output_file, 0);
    
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, input_file)) != -1) {
            line[read-1] = 0;
            char *context = strdup(line);
            char *firstBlank = line;
            while (*firstBlank != 0 && *(firstBlank++) != ' ') {}
            *(firstBlank-1) = 0;
            print_actions(state_machine_dispatch_event(sm,line), output_file, context);
            free(context);
    }
    
    if (line) free(line);
    
    state_machine_delete(sm);
    
    fclose(input_file);
    fclose(output_file);
    return 0;
}
