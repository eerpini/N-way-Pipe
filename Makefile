all:
	gcc -g -o npipe parse.c main.c
clean:
	rm -rf npipe a.out
