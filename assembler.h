#pragma once
#include <stdio.h>
#include "src/str.h"

enum Command_en {PUSH_ASS = 1,
                 ADD_ASS  = 2,
                 SUB_ASS  = 3,
                 MUL_ASS  = 4,
                 DIV_ASS  = 5,
                 OUT_ASS  = 6,
                 HET_ASS  = 7};

/*! 
    Print in bytecode file (binary)
*/
void Print(long long cond, FILE* bytecode);

/*!
    Translate to bytecode and print it 
*/
void Translator(struct Text_t* assm_file, FILE* bytecode);