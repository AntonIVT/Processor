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
                 
void Print_Command(char command, FILE* bytecode);

void Print_Value(double value, FILE *bytecode);

void Assembler(const char* assm_file, const char* bytecode_file);

void Disassembler(const char *assm_file, const char *bytecode_file);

void Listing(const char *bytecode_file, const char *listing_file);