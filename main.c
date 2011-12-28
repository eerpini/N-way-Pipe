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
        int ostdin, ostdout;
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
                        //close(pipes[i][1]);
                        for(j=0; j<argc-1; j++){
                                /*
                                if(j==i)
                                     continue;   
                                     */
                                close(pipes[j][0]);
                                close(pipes[j][1]);
                        }
                       if( execvp((const char *)args_for_exec[0], (char **)args_for_exec) == -1){
                               perror("execvp");
                               exit(EXIT_FAILURE);
                       }
                       break;
                }

        }


        close(pipes[0][1]);
        for(i=1; i<argc-1; i++){
                close(pipes[i][0]);
        }

        child_count++;
        broker = fork();
        if(broker == 0){
                int rcount = -1;
                //char buf[BUFSIZE] = {0};
                char buf = 0;
                while((rcount = read(pipes[0][0], &buf, 1))){
                        for(i=1; i<argc-1; i++){
                                write(pipes[i][1], &buf, 1);
                        }
                }
                /*
                while(1){
                        for(i=0; i< BUFSIZE; i++){
                                rcount = read(pipes[0][0], &buf[i], 1);
                                if(rcount <= 0){
                                        break;
                                }
                        }
                        if(i<BUFSIZE){
                                if(i>0){
                                        for(j=1; j<argc-1; j++){
                                                write(pipes[i][1], buf, i);
                                        }
                                }
                                break;
                        }
                        else{
                                for(j=1; j<argc-1; j++){
                                        write(pipes[i][1], buf, i);
                                }
                        }
                
                }
                */
                //fprintf(stderr, "Done reading\n");
                /*
                buf = EOF;

                for(i=1; i<argc-1; i++){
                        write(pipes[i][1], &buf, 1);
                }
                */
                //fprintf(stderr, "Done writing EOF\n");

                close(pipes[0][0]);
                for(i=1; i<argc-1; i++){
                        close(pipes[i][1]);
                }
                //fprintf(stderr, "Done closing pipes\n");
                
                //fprintf(stderr, "Done multiplexing\n");
        }

        /*
         * Wait for the child to finish in the parent
         */
        else{
                do{
                        tpid = wait(&child_status);
                        child_count--;
                        /*
                        if(tpid == writer){
                                fprintf(stderr, "Writer done\n");
                        }
                        */
                        if(tpid == broker){
                                //fprintf(stderr, "Broker done, quitting\n");
                                break;
                        }
                        /*
                         * this condition is not good enough, needs to be fixed
                         */
                        /*
                        if(tpid == child)
                                break;
                                */
                }while(child_count > 0);
        }

        return EXIT_SUCCESS;
}
