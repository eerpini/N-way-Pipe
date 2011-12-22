all:
	gcc -g -o npipe parse.c fork_utils.c main.c
clean:
	rm -rf npipe a.out
