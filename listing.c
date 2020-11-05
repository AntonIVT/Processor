#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "src/str.h"
#include "assembler.h"
#include "enum_commands.h"
#include "enum_registers.h"

#define VER 8

//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------LISTING------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------

void Listing_command(char **rip, int type, FILE *listing)
{
    assert(rip != NULL);
    assert(listing != NULL);
    
    switch(type)
    {
        case MULTI :
        {
            char type = *((*rip)++);
            fprintf(listing, " (0x%d%d%d)    | ", (type & RAM_ARG) / RAM_ARG, (type & REG_ARG) / REG_ARG, type & IMMED_ARG);
            if (type & REG_ARG)
            {
                fprintf(listing, "  %d (0x%02X) ",  **rip, **rip);
                *rip += sizeof(char);
            }
            else
                fprintf(listing, "%11c", ' ');
            
            if (type & IMMED_ARG)
            {
                fprintf(listing, "%4lg (0x%016llX) | ", *(double *)*rip, *(long long *)*rip);
                *rip += sizeof(double);
            }
            else
                fprintf(listing, "%26c| ", ' ');
            break;
        }
        case VALUE :                                         // Таких, в общем-то нет
            fprintf(listing, "%8lg (0x%016llX) | ", *(double *)*rip, *(long long *)*rip);
            *rip += VALUE;
            break;
        case REGISTER :
            fprintf(listing, "%11c | %3d (0x%02X) %25c | ", ' ' , **rip,**rip , ' ');
            *rip += REGISTER;
            break;
        case NONE :
            fprintf(listing, "%11c | %36c | ", ' ', ' ');
            break;
        case LABEL :
            fprintf(listing, "%11c | %3d (0x%03X) %24c | ", ' ', *(short *)*rip, *(short *)*rip, ' ');
            *rip += LABEL;
            break;
        default :
            printf("BAN IN LISTING_LOW \n");
            break;
    }
}

//-----------------------------------------------------------------------------------------------------------------------

#define DEF_CMD( enum_name, assm_name, enum_code, argument_type, code )              \
    case CMD_ ## enum_name :                                                         \
    {                                                                                \
        Listing_command(&rip, argument_type, listing);                               \
        fprintf(listing, # assm_name "\n");                                          \
        break;                                                                       \
    }

//-----------------------------------------------------------------------------------------------------------------------

void Listing(const char *bytecode_file, const char *listing_file)
{
    assert(bytecode_file != NULL);
    assert(listing_file  != NULL);
    
    FILE *bytecode = fopen(bytecode_file, "rb");
    FILE *listing  = fopen(listing_file,  "wb");
    
    assert(listing != NULL);
    
    char *buffer = FileToBuffer(bytecode);
    int count_bytes = FileSize(bytecode);
    char *rip = buffer;
    fclose(bytecode);
    
    const char *signature = rip;
    rip += 2;
    char version = *rip;
    rip++;
    
    fprintf(listing, "SIGNATURE %.2s, VERSION %d\n", signature, version);
    
    fprintf(listing, "Adress | Command(hex) Type(push)  | Value#1              Value#2         | ASS_command\n"
                     "--------------------------------------------------------------------------\n");
                     
    while (rip < buffer + count_bytes)
    {
        fprintf(listing, "0x%04X | ", rip - buffer);
        unsigned char command = *rip;
        
        fprintf(listing, "%3d   (0x%02X) ", command, command);
        
        rip++;
        switch (command)
        {
        #include "COMMANDS.H"
        default :
            printf("BAN IN LISTING\n");
            printf("UNKNOWN COMMAND : %d %d\n", command, rip - buffer);
            break;
        }
    }
    free(buffer);
    fclose(listing);
}
#undef DEF_CMD

//-----------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    Listing(argv[1], argv[2]);
}