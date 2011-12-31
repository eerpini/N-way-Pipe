all:
	gcc -g -o npipe parse.c fork_utils.c npipe.c
	make -C utils/
clean:
	rm -rf npipe a.out
	make clean -C utils/
