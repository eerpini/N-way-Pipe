all:
	gcc -g -o npipe parse.c
clean:
	rm -rf npipe a.out
