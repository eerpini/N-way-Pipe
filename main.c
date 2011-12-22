#include "npipe.h"
/*
 *
 * Tue Dec 20 23:47:01 EST 2011
 * Satish Kumar Eerpini 
 */


int main(int argc, char *argv[]){
        /*
         * Core parsing logic, look for `` in the arguments
         */
        if(argc < 3){
                fprintf(stderr, "Invalid args\n");
                return EXIT_FAILURE;
        }

        pid_t child = 0, tpid = 0;
        int i= 0;
        int count = 0;
        int child_status = 0;
        char ** args_for_exec = NULL;
        fprintf(stdout, "The number of args is : [%d]\n", count);

        int * prog_exec_ind = (int *)break_by_spaces(argv[1], &count);
        if(prog_exec_ind == NULL){
                fprintf(stderr, "Cannot exec program specified as first arg\n");
                return EXIT_FAILURE;
        }
        args_for_exec = build_argv(argv[1], prog_exec_ind, count);

       child = fork();
        if(child == 0){
               if( execvp((const char *)args_for_exec[0], (char **)args_for_exec) == -1){
                       perror("execvp");
                       exit(EXIT_FAILURE);
               }
        }
        /*
         * Wait for the child to finish in the parent
         */
        else{
                do{
                        tpid = wait(&child_status);
                }while(tpid != child);
        }
        return EXIT_SUCCESS;
}
