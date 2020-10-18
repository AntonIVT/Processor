All:
	ls
comp:
	gcc -o compailer compailer.c src/*.c
proc:
	gcc -o processor processor.c src/*.c 