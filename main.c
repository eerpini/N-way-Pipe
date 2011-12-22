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
        int i= 0;
        int count = 0;
        int child_status = 0;
        char ** args_for_exec = NULL;
        int pipes[argc-1][2];
        int ostdin, ostdout;
        ostdin =  dup(0);
        ostdout = dup(1);
        for(i=0; i< argc-1; i++){
                if(pipe(pipes[i]) < 0){
                        perror("pipe");
                        exit(EXIT_FAILURE);
                }
        }

        int * prog_exec_ind = (int *)break_by_spaces(argv[1], &count);
        fprintf(stdout, "The number of args is : [%d]\n", count);
        if(prog_exec_ind == NULL){
                fprintf(stderr, "Cannot exec program specified as first arg\n");
                return EXIT_FAILURE;
        }
        args_for_exec = build_argv(argv[1], prog_exec_ind, count);
        writer = fork();

        if(writer == 0){

                if(dup2(pipes[0][1], 1) == -1){
                        perror("dup2");
                        exit(EXIT_FAILURE);
                }
                close(pipes[0][0]);
                for(i=1; i<argc-1; i++){
                        close(pipes[i][0]);
                        close(pipes[i][1]);
                }
               if( execvp((const char *)args_for_exec[0], (char **)args_for_exec) == -1){
                       perror("execvp");
                       exit(EXIT_FAILURE);
               }
        }
        
        /* Now spawn each of the reader processes */
        for(i=1; i<argc-1; i++){

                count = 0;
                prog_exec_ind = NULL;
                prog_exec_ind = (int *)break_by_spaces(argv[i+1], &count);
                fprintf(stdout, "The number of args is : [%d]\n", count);
                if(prog_exec_ind == NULL){
                        fprintf(stderr, "Cannot exec program specified as first arg\n");
                        return EXIT_FAILURE;
                }
                args_for_exec = NULL;
                args_for_exec = build_argv(argv[i+1], prog_exec_ind, count);
                child = fork();
                if(child == 0){
                        if(dup2(pipes[i][0], 0) == -1){
                                perror("dup2");
                                exit(EXIT_FAILURE);
                        }
                        close(pipes[i][1]);
                        int j;
                        for(j=0; j<argc-1; j++){
                                if(j==i)
                                     continue;   
                                close(pipes[j][0]);
                                close(pipes[i][1]);
                        }
                       if( execvp((const char *)args_for_exec[0], (char **)args_for_exec) == -1){
                               perror("execvp");
                               exit(EXIT_FAILURE);
                       }
                }

        }


        close(pipes[0][1]);
        for(i=1; i<argc-1; i++){
                close(pipes[i][0]);
        }

        if(fork() == 0){
                int rcount = -1;
                char buf = 0;
                while((rcount = read(pipes[0][0], &buf, 1))>=0){
                        if(rcount == 0)
                                continue;
                        for(i=1; i<argc-1; i++){
                                write(pipes[i][1], &buf, 1);
                        }
                }
                
                buf = EOF;

                for(i=1; i<argc-1; i++){
                        write(pipes[i][1], &buf, 1);
                }

        }

        /*
         * Wait for the child to finish in the parent
         */
        else{
                do{
                        tpid = wait(&child_status);
                }while(tpid != writer);
        }
        return EXIT_SUCCESS;
}
