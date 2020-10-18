#pragma once
#include <stdio.h>
#include "src/str.h"

enum Command_en {PUSH = 1, // Действия с числами
                 ADD  = 2,
                 SUB  = 3,
                 MUL  = 4,
                 DIV  = 5,
                 OUT  = 0xC1, // Общие Команды (Commands)
                 HET  = 0xC2};

/*! 
    Print in bytecode file (binary)
*/
void Print(long long cond, FILE* bytecode);

/*!
    Translate to bytecode and print it 
*/
void Translator(struct Text_t* assm_file, FILE* bytecode);