#include <stdio.h>
#include <string.h>
#include "src/str.h"
#include "assembler.h"

void Print(long long cond, FILE* bytecode)      
{                                                      
        long long tmp = cond;                   
        fwrite(&tmp, sizeof(long long), 1, bytecode);  
}

void Translator(struct Text_t* assm_file, FILE* bytecode)
{
    for (int i = 0; i < assm_file->countoflines; i++)
    {
        if (strstr(assm_file->lines[i].pointer, "push"))
        {    
            Print(PUSH_ASS, bytecode);
            
            double tmp_value_double = 0;
            sscanf(assm_file->lines[i].pointer + 4, "%lg", &tmp_value_double);
            Print((long long)(tmp_value_double * 256), bytecode);
        }
        if (strstr(assm_file->lines[i].pointer, "add"))
            Print(ADD_ASS, bytecode);
        if (strstr(assm_file->lines[i].pointer, "sub"))
            Print(SUB_ASS, bytecode);
        if (strstr(assm_file->lines[i].pointer, "mul"))
            Print(MUL_ASS, bytecode);
        if (strstr(assm_file->lines[i].pointer, "div"))
            Print(DIV_ASS, bytecode);
        if (strstr(assm_file->lines[i].pointer, "out"))
            Print(OUT_ASS, bytecode);
        if (strstr(assm_file->lines[i].pointer, "het"))
            Print(HET_ASS, bytecode);    
    }
}

int main(int argc, char **argv)
{
    const char *assembler_file  = "assm.txt";
    const char *bytecode_file = "bytecode.h";
    
    if (argc > 2)
    {
        assembler_file  = argv[1];
        bytecode_file = argv[2];
    } 
    else if (argc == 2)
        assembler_file  = argv[1];
    
    struct Text_t assm_file = {0};
    TextConstruct(&assm_file, assembler_file);
    
    FILE *bytecode = fopen(bytecode_file , "wb");
    Translator(&assm_file, bytecode);
    
    TextDeconstruct(&assm_file);
}