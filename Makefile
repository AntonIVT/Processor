All:
	ls
comp:
	gcc -o compiler compiler.c src/*.c
proc:
	gcc -o processor processor.c src/*.c 