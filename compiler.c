/* -добавить переменные
   -квадратка
   */
  
  
#define VER 1
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "src/str.h"
#include "assembler.h"

void Print_Command(char command, FILE* bytecode)      
{                                                                        
    fwrite(&command, sizeof(char), 1, bytecode);  
}

void Print_Value(double value, FILE *bytecode)
{
    fwrite(&value, sizeof(double), 1, bytecode);
}

void Assembler(const char* assm_file, const char* bytecode_file)
{
    assert(assm_file     != NULL);
    assert(bytecode_file != NULL);
    
    struct Text_t assm = {0};
    TextConstruct(&assm, assm_file);
    
    FILE *bytecode = fopen(bytecode_file, "wb");
    assert(bytecode != NULL);
    
    fprintf(bytecode, "UT%c", VER);
    
    for (int i = 0; i < assm.countoflines; i++)
    {
        if (strstr(assm.lines[i].pointer, "push"))
        {    
            Print_Command(PUSH, bytecode);
            
            double tmp_value = 0;
            sscanf(assm.lines[i].pointer + 4, "%lg", &tmp_value);
            Print_Value(tmp_value, bytecode);
        }
        if (strstr(assm.lines[i].pointer, "add"))
            Print_Command(ADD, bytecode);
        if (strstr(assm.lines[i].pointer, "sub"))
            Print_Command(SUB, bytecode);
        if (strstr(assm.lines[i].pointer, "mul"))
            Print_Command(MUL, bytecode);
        if (strstr(assm.lines[i].pointer, "div"))
            Print_Command(DIV, bytecode);
        if (strstr(assm.lines[i].pointer, "out"))
            Print_Command(OUT, bytecode);
        if (strstr(assm.lines[i].pointer, "het"))
            Print_Command(HET, bytecode);    
    }
    TextDeconstruct(&assm);
    fclose(bytecode);
}

void Disassembler(const char *assm_file, const char *bytecode_file)
{
    assert(assm_file     != NULL);
    assert(bytecode_file != NULL);
    
    FILE *bytecode = fopen(bytecode_file, "rb");
    FILE *assm     = fopen(assm_file, "wb");
    
    assert(assm != NULL);
    
    char *buffer = FileToBuffer(bytecode);
    int count_bytes = FileSize(bytecode);
    char *curr_ptr = buffer;
    
    const char *signature = curr_ptr;
    curr_ptr += 2;
    char version = *curr_ptr;
    curr_ptr++;
    
    if (!strcmp(signature, "UT"))
    {
        printf("SORRY, WRONG SIGNATURE, CANT DISASS :(((\n");
        return;
    }
    if (version != VER)
    {
        printf("%d %d\n", VER, version);
        printf("SORRY, YOUR BYTECODE HAS OLD VERSION, CANT DIASS :(((\n");
        return;
    }
    
    while (curr_ptr < buffer + count_bytes)
    {
        unsigned char command = *curr_ptr;
        double tmp1 = 0;
        curr_ptr += sizeof(char);
        switch (command)
        {
        case PUSH :
            fprintf(assm, "push %lg\n", *(double *)curr_ptr);
            curr_ptr += sizeof(double);
            break;
        case ADD :
            fprintf(assm, "add\n");
            break;
        case SUB :
            fprintf(assm, "sub\n");
            break;
        case MUL :
            fprintf(assm, "mul\n");
            break;
        case DIV :
            fprintf(assm, "div\n");
            break;
        case OUT :
            fprintf(assm, "out\n");
            break;
        case HET :
            fprintf(assm, "het\n");
            break;
        default :
            printf("BAN\n");
            break;
        }
    }
    free(buffer);
    fclose(bytecode);
    fclose(assm);
}

void Listing(const char *bytecode_file, const char *listing_file)
{
    assert(bytecode_file != NULL);
    assert(listing_file  != NULL);
    
    FILE *bytecode = fopen(bytecode_file, "rb");
    FILE *listing  = fopen(listing_file,  "wb");
    
    assert(listing != NULL);
    
    char *buffer = FileToBuffer(bytecode);
    int count_bytes = FileSize(bytecode);
    char *curr_ptr = buffer;
    
    const char *signature = curr_ptr;
    curr_ptr += 2;
    char version = *curr_ptr;
    curr_ptr++;
    fprintf(listing, "SIGNATURE %.2s, VERSION %d\n", signature, version);
    
    fprintf(listing, "Adress | Command  Value | Com  Value\n"
                     "-------------------------------------------------\n");
                     
    while (curr_ptr < buffer + count_bytes)
    {
        fprintf(listing, "0x%04X | ", curr_ptr - buffer);
        unsigned char command = *curr_ptr;
        fprintf(listing, "%3d ", command);
        double tmp1 = 0;
        curr_ptr += sizeof(char);
        switch (command)
        {
        case PUSH :
            fprintf(listing, "%10lg | 0x%02X 0x%016llX\n", *(double *)curr_ptr, command, *(unsigned long long *)curr_ptr ); // Строит уже в привычной big endian
            curr_ptr += sizeof(double);
            break;
        case ADD : case SUB : case MUL :
        case DIV : case OUT : case HET :
            fprintf(listing, "%10c | 0x%02X\n", ' ', command);
            break;
        default :
            printf("BAN\n");
            break;
        }
    }
    free(buffer);
    fclose(bytecode);
    fclose(listing);
}

int main(int argc, char **argv)
{
    const char *input_file  = NULL;
    const char *output_file = "bytecode.code";    // bytecode.h зарезервировано 
    const char *mode = NULL;                      // -a == assembler, -d == disassembler
    const char *listing = NULL;                   // -lst == будет произведен листинг байткода
    
    char is_input_file_named  = 0;
    char is_output_file_named = 0;
    
    for (int i = 1; i < argc && i < 5; i++)
    {
        if (argv[i][0] == '-')
        {
            if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "-d"))
                mode = argv[i];
            else if (!strcmp(argv[i], "-lst"))
                listing = argv[i];
            else
                printf("DEBIL\n"
                       "wrong compailer syntax, no such flag : %s\n", argv[i]);
        }
        else
        {
            if (!is_input_file_named)
            {
                input_file = argv[i];
                is_input_file_named = 1;
            }
            else if (!is_output_file_named)
            {
                output_file = argv[i];
                is_output_file_named = 1;
            }
            else
                printf("DEBIL\n"
                       "wrong compailer syntax, extra argument : %s\n", argv[i]);
        }
    }
    
    
    
    if (mode != NULL && listing != NULL && !strcmp(mode, "-a") && !strcmp(listing, "-lst"))     // Assembler with listing
    {
        Assembler(input_file, output_file);
        Listing(output_file, "bytecode.lst");
    }
    else if (mode != NULL && !strcmp(mode, "-a") && listing == NULL)         // Assembler without listing
        Assembler(input_file, output_file);
        
    else if (mode != NULL && !strcmp(mode, "-d") && listing == NULL)         // Disassembler without listing
    {    
        if (!strcmp(output_file, "bytecode.code"))
            output_file = "disassemblered.assm";
        Disassembler(output_file, input_file);
    }    
    else if (listing != NULL && !strcmp(listing, "-lst") && mode == NULL)       // Just listing input file
        Listing(input_file, "bytecode.lst");
        
    else
        printf("DEBIL\n"
                       "wrong compailer syntax\n");
}