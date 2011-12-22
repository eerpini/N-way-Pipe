#include "npipe.h"
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
