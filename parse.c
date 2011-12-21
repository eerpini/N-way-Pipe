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
int * break_by_spaces(char * arg){

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
        arg++; 
        counter++;
        while(*arg){
                if(*arg == ' '){
                        *(++temp) = counter+1;
                        *arg = 0;
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

        int i= 0;
        int * prog_exec_ind = break_by_spaces(argv[1]);

        if(prog_exec_ind == NULL){
                fprintf(stderr, "Cannot exec program specified as first arg\n");
                return EXIT_FAILURE;
        }

        for(i=0; *(prog_exec_ind + i) != -1; i++){
                fprintf(stdout, "[%d] %d %s \n", i, *(prog_exec_ind + i), (char *)(argv[1] + *(prog_exec_ind + i)));
        }

        return EXIT_SUCCESS;

}
