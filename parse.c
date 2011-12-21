#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 *
 * Tue Dec 20 23:47:01 EST 2011
 * Satish Kumar Eerpini 
 */

/*
 * Returned the indices of the words in the given sentence
 * with space as the default separator
 */
int * break_by_spaces(char * arg, int * num_args){

        if(strlen(arg) <= 0 || (strlen(arg) ==1 && *arg == ' ')){
                return NULL;
        }

        int * retval = malloc((strlen(arg) -1)*sizeof(int));
        int * temp = retval;
        int counter = 0;
        if(retval == NULL){
                perror("malloc");
                exit(EXIT_FAILURE);
        }
        memset(retval, 0, (strlen(arg) -1)*sizeof(int));

        *temp = counter;
        *num_args = *num_args + 1;
        arg++; 
        counter++;
        while(*arg){
                if(*arg == ' '){
                        *(++temp) = counter+1;
                        *arg = 0;
                        *num_args = *num_args + 1;
                }
                arg++;
                counter++;
        }
        *(++temp) = -1;
        return retval;
}

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
        int * prog_exec_ind = break_by_spaces(argv[1], &count);
        int child_status = 0;
        fprintf(stdout, "The number of args is : [%d]\n", count);
        char *args_for_exec[count+1];

        if(prog_exec_ind == NULL){
                fprintf(stderr, "Cannot exec program specified as first arg\n");
                return EXIT_FAILURE;
        }

        //We can also use i < count for the condition in the for block
        for(i=0; *(prog_exec_ind + i) != -1; i++){
                fprintf(stdout, "[%d] %d %s \n", i, *(prog_exec_ind + i), (char *)(argv[1] + *(prog_exec_ind + i)));
                *(args_for_exec+ i) = strdup((char *)(argv[1] + *(prog_exec_ind + i))); 
        }
        *(args_for_exec + count) = NULL;
        for(i=0; i< count-1; i++){
                fprintf(stdout, "[%d] %s\n", i, *(args_for_exec+i));
        }

        child = fork();
        if(child == 0){
               if( execvp((char *)args_for_exec[0], (char **)args_for_exec) == -1){
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
