all:
	dir
assm_win:
	gcc -o assembler.exe assembler.c src/*.c
	copy assembler.exe bin_win
	del assembler.exe
list_win:
	gcc -o listing.exe listing.c src/*.c
	copy listing.exe bin_win
	del listing.exe
disass_win:
	gcc -o disassembler.exe disassembler.c src/*.c
	copy disassembler.exe bin_win
	del disassembler.exe
proc_win:
	g++ -o processor.exe processor.cpp src/*.c
	copy processor.exe bin_win
	del processor.exe
utc_win:
	gcc -o utc.exe utc.c
	copy utc.exe bin_win
	del utc.exe
project_win: assm_win list_win disass_win proc_win utc_win

assm_lin:
	gcc -o assembler assembler.c src/*.c
	cp assembler bin_lin/
	rm assembler
list_lin:
	gcc -o listing listing.c src/*.c
	cp listing bin_lin/
	rm listing
disass_lin:
	gcc -o disassembler disassembler.c src/*.c
	cp disassembler bin_lin/
	rm disassembler
proc_lin:
	g++ -o processor processor.cpp src/*.c -lsfml-graphics -lsfml-window -lsfml-system
	cp processor bin_lin/
	rm processor
utc_lin:
	gcc -o utc utc.c
	cp utc bin_lin/
	rm utc
project_lin: assm_lin list_lin disass_lin proc_lin utc_lin

dd:
	g++ -o dd dd.cpp -lsfml-graphics -lsfml-window -lsfml-system
	

	
