#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "assembler.h"
#include "enum_commands.h"
#include "enum_registers.h"

#define VER 8

const int count_of_labels = 200;

//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------DISASSEMBLER-------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------

void PrintArg(char **rip, FILE* assm, int argument_type, int *curr_label, struct label *labels)
{
    switch(argument_type)
    {
        case LABEL:
        {
            int flag = 0;
            
            for (int i = 0; i < count_of_labels; i++)
            {
                if (*((short *)*rip) == labels[i].offset)
                {
                    fprintf(assm, "%s", labels[i].name);
                    flag = 1;
                    break;
                }
            }
            
            if (!flag)
            {
                labels[*curr_label].offset = *((short *)*rip);
                (*curr_label)++;
            }
            
            *rip += sizeof(short);
            break;
        }
        case VALUE:
        {
            fprintf(assm, "%+lg", *((double *)*rip));
            *rip += sizeof(double);
            break;
        }
        case REGISTER:
        {
            int reg = *((*rip)++);
            #define DEF_REG(enum_name, name, number)\
                case number: fprintf(assm, "%s", #name);break; 
            switch(reg)
            {
            #include "REGISTERS.H"
                default:
                    printf("WRONG REGISTER\n");
                    break;
            }
            #undef DEF_REG            
            break;
        }
        case MULTI:
        {
            char type = *((*rip)++);
            
            if (type & RAM_ARG)
                fprintf(assm, "[");
            if (type & REG_ARG)
                PrintArg(rip, assm, REGISTER, curr_label, labels);
            if (type & IMMED_ARG)
                PrintArg(rip, assm, VALUE, curr_label, labels);
            if (type & RAM_ARG)
                fprintf(assm, "]");
        }
        case NONE:
            break;
    }
}

//-----------------------------------------------------------------------------------------------------------------------

#define DEF_CMD( enum_name, assm_name, enum_code, argument_type, code )                 \
    case enum_code:                                                                     \
        fprintf(assm, "%s ", #assm_name);                                               \
        PrintArg(&rip, assm, argument_type, &curr_label, labels);                       \
        fprintf(assm, "\n");                                                            \
        break;                                                                       

//-----------------------------------------------------------------------------------------------------------------------

int Filling(char *buffer, int count_of_bytes, struct label *labels, FILE* assm)
{
    char *rip = buffer;
    rip += 3;
    int curr_label = 0;
    int curr_lb = 0;
    while (rip - buffer < count_of_bytes)
    {
        if (labels[curr_lb].offset == rip - buffer)
        {
            fprintf(assm, "%s:\n", labels[curr_lb].name);
            curr_lb++;
        }
        
        unsigned char command = *rip;
        rip++;
        switch(command)
        {
            #include "COMMANDS.H"
            default:
                printf("BAN IN DIASSEMBLER, rip = %d\n");
                printf("UNKNOWN COMMAND : %lli\n", rip - buffer, command);
                break;
        }    
    }
    return curr_label;
}

//-----------------------------------------------------------------------------------------------------------------------

int BytecodeVerify(char *rip)
{
    assert(rip != NULL);
    
    char *signature = (char *)calloc(2, sizeof(char));
    strncpy(signature, rip, 2);
    
    rip += 2;
    char version = *rip;
    
    if (version == VER && !strcmp(signature, "UT"))
    {   
        free(signature);
        return 1;
    }
    else
    {
        free(signature);
        return 0;
    }
}

//-----------------------------------------------------------------------------------------------------------------------

int LabelsCompare(const void *left, const void *right)
{
    if (((struct label *)left)->offset == -1)
        return 1;
    if (((struct label *)right)->offset == -1)
        return -1;
    return (((struct label *)left)->offset - ((struct label *)right)->offset);
}

//-----------------------------------------------------------------------------------------------------------------------

void Disassembler(const char *bytecode_file, const char *assm_file)
{
    assert(assm_file     != NULL);
    assert(bytecode_file != NULL);
    
    FILE *bytecode = fopen(bytecode_file, "rb");
    
    char *buffer = FileToBuffer(bytecode);
    int count_bytes = FileSize(bytecode);
    
    fclose(bytecode);
    
    if (!BytecodeVerify(buffer))
    {
        printf("WRONG VERSION OR SIGNATURE\n");
        return;
    }
    
    struct label *labels = (struct label *)calloc(count_of_labels, sizeof(struct label));  
        
    for (int i = 0; i < count_of_labels; i++)                                               
    {
        labels[i].offset = -1;
        labels[i].name = (char *)calloc(10, sizeof(char));
        strcat(labels[i].name, "label");
        sprintf(labels[i].name + 5, "%d", i + 1);                                                    
    }
    
    FILE *assm = fopen(assm_file, "wb");
    int labels_ = Filling(buffer, count_bytes, labels, assm);
    fclose(assm);
    
    qsort(labels, labels_, sizeof(struct label), LabelsCompare);
    
    assm = fopen(assm_file, "wb");
    Filling(buffer, count_bytes, labels, assm);
    fclose(assm);
    
    for (int i = 0; i < count_of_labels; i++)
    {
        free(labels[i].name);
    }
    
    free(labels);
    free(buffer);
}

//-----------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    Disassembler(argv[1], argv[2]);
}