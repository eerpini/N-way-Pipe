#include "npipe.h"

char ** build_argv(char *command_string, int * prog_exec_ind, int count){

        char ** args_for_exec = malloc(sizeof(char *)*(count + 1));

        int i;

        //We can also use i < count for the condition in the for block
        for(i=0; *(prog_exec_ind + i) != -1; i++){
                fprintf(stdout, "[%d] %d %s \n", i, *(prog_exec_ind + i), (char *)(command_string + *(prog_exec_ind + i)));
                *(args_for_exec+ i) = (char *) strdup((char *)(command_string + *(prog_exec_ind + i))); 
        }
        *(args_for_exec + count) = (char *)NULL;
        for(i=0; i< count-1; i++){
                fprintf(stdout, "[%d] %s\n", i, *(args_for_exec+i));
        }

        return args_for_exec;
}

 
