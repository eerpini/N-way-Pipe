#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
/*
 * Works like cat when there are not arguments, reads from stdin
 * and writes to stdout
 * Takes only one argument, opens a file with that name and reads 
 * from stdin and writes to the file
 */

int main(int argc, char *argv[]){

        char buf = 0;

        if(argc == 1){

                while(read(0, &buf, 1)){
                        write(1, &buf, 1);
                }
        }
        else{
                if(argc > 2){
                        fprintf(stderr, "Usage : %s [filename]\n", argv[0]);
                }
                int fd = open(argv[1], O_CREAT|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
                while(read(0, &buf, 1)){
                        write(fd, &buf, 1);
                }
                close(fd);
        }

        return EXIT_SUCCESS;

}
