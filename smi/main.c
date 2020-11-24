#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sm/state_machine.h>
#include <sm/state_machine_definition.h>
#include "cmdline.h"
#include "parsing.h"
#include "validate.h"
#include "enumerate.h"

void print_actions(list_t * l, FILE* out)
{
    list_element_t * iterator = 0;
    while((iterator = list_find_next_element(l,iterator)) != 0)
    {
        char* action = (char*)list_get_data(iterator);
        fprintf(out,"%s\n",action);
        fflush(out);
    }
    list_delete(l);
}

int main(int argc, char* argv[])
{
    struct gengetopt_args_info args_info;
    if (cmdline_parser (argc, argv, &args_info) != 0) return 1;
    
    FILE *definition_file = fopen(args_info.definition_arg, "r");
    

    if (!definition_file)
    {
        fprintf(stderr,"Unable to open definition file %s\n",
                args_info.definition_arg);
        cmdline_parser_free(&args_info);
        return -1;
    }
    
    if (args_info.validate_only_given)
    {
        cmdline_parser_free(&args_info);
        return validate(definition_file) == 0;
    }
    else if (args_info.list_states_given)
    {
        cmdline_parser_free(&args_info);
        return enumerate_states(definition_file) == 0;
    }
    else if (args_info.list_events_given)
    {
        cmdline_parser_free(&args_info);
        return enumerate_events(definition_file) == 0;
    }
    else if (args_info.list_actions_given)
    {
        cmdline_parser_free(&args_info);
        return enumerate_actions(definition_file) == 0;
    }

    FILE *input_file = fopen(args_info.input_arg, "r");
    if (!input_file)
    {
        fprintf(stderr,"Unable to open input file %s\n",args_info.input_arg);
        cmdline_parser_free(&args_info);
        fclose(definition_file);
        return -1;
    }
    
    FILE *output_file = fopen(args_info.output_arg, "w");
    if (!output_file)
    {
        fprintf(stderr,"Unable to open output file %s\n",args_info.output_arg);
        cmdline_parser_free(&args_info);
        fclose(definition_file);
        fclose(input_file);
        return -1;
    }
    
    cmdline_parser_free(&args_info);

    state_machine_t * sm;
    sm = state_machine_new(0);
    
    if(!parse(definition_file, sm))
    {
        state_machine_delete(sm);
        fclose(definition_file);
        fclose(input_file);
        fclose(output_file);
        return -1;
    }

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
