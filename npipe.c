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

        pid_t child = 0, tpid = 0, writer = 0;
        pid_t broker = 0;
        int i= 0;
        int j=0;
        int child_count = 0;
        int count = 0;
        int child_status = 0;
        char ** args_for_exec = NULL;
        int pipes[argc-1][2];

        //Variables used for redirecting the output
        int rcount = -1;
        char buf = 0;

        for(i=0; i< argc-1; i++){
                if(pipe(pipes[i]) < 0){
                        perror("pipe");
                        exit(EXIT_FAILURE);
                }
        }

        int * prog_exec_ind = (int *)break_by_spaces(argv[1], &count);
        if(prog_exec_ind == NULL){
                fprintf(stderr, "Cannot exec program specified as first arg\n");
                return EXIT_FAILURE;
        }
        args_for_exec = build_argv(argv[1], prog_exec_ind, count);
        child_count++;
        writer = fork();

        if(writer == 0){
                if(dup2(pipes[0][1], 1) == -1){
                        perror("dup2");
                        exit(EXIT_FAILURE);
                }
                close(pipes[0][0]);
                close(pipes[0][1]);

                for(i=1; i<argc-1; i++){
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                }
               if( execvp((const char *)args_for_exec[0], (char **)args_for_exec) == -1){
                       perror("execvp");
                       exit(EXIT_FAILURE);
               }

               exit(EXIT_FAILURE);
        }
#ifdef DEBUG
        printf("[DEBUG] The writer PID is %u\n", writer);
#endif
        /* Now spawn each of the reader processes */
        for(i=1; i<argc-1; i++){
                count = 0;
                prog_exec_ind = NULL;
                prog_exec_ind = (int *)break_by_spaces(argv[i+1], &count);
                if(prog_exec_ind == NULL){
                        fprintf(stderr, "Cannot exec program specified as first arg\n");
                        return EXIT_FAILURE;
                }
                args_for_exec = NULL;
                args_for_exec = build_argv(argv[i+1], prog_exec_ind, count);
                child_count++;
                child = fork();
                if(child == 0){
                        if(dup2(pipes[i][0], 0) == -1){
                                perror("dup2");
                                exit(EXIT_FAILURE);
                        }
                        for(j=0; j<argc-1; j++){
                                close(pipes[j][0]);
                                close(pipes[j][1]);
                        }
                       if( execvp((const char *)args_for_exec[0], (char **)args_for_exec) == -1){
                               perror("execvp");
                               exit(EXIT_FAILURE);
                       }
                       break;
                }
#ifdef DEBUG
        printf("[DEBUG] The reader PID is %u\n", child);
#endif
        }


        //Close the duplicate write end of the writer
        close(pipes[0][1]);

        child_count++;
        while((rcount = read(pipes[0][0], &buf, 1))){
                for(i=1; i<argc-1; i++){
                        write(pipes[i][1], &buf, 1);
                }
        }

#ifdef DEBUG
        fprintf(stderr, "[DEBUG] Done reading from writer\n");
#endif

        //Close all the remaining file descriptors
        for(i=0; i<argc-1; i++){
                close(pipes[i][0]);
                close(pipes[i][1]);
        }

        /*
         * Wait for the children to finish before quitting
         */
        do{
#ifdef DEBUG
                fprintf(stderr, "[DEBUG] Waiting on the child to quit\n");
#endif
                tpid = wait(&child_status);
                child_count--;
        }while(child_count > 0);

        return EXIT_SUCCESS;
}
