All:
	ls
assm:
	gcc -o assembler assembler.c src/*.c
proc:
	make assm
	./assembler
	gcc -o processor processor.c src/*.c 