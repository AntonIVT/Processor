# Processor
Home made processor with girls and Black Jack
##FAQ
- There is compiler and processor
- Language is very simple (push, add, mul, etc)
- To compile compiler or processor check Makefile
- To compile your program : ./compiler -a <input_file> <output_file>
- Flags:
  - -a compailing your program to bytecode
  - -d disassemblering bytecode 
  - -lst doing listing file of bytecode
    - if you use it with -a, your new bytecode will be listined
    - you can use just -lst, you need to point bytecode file : ./compiler -lst <bytecode_file>
