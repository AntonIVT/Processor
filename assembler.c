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
//----------------------------------------------ASSEMBLER----------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------

int SetLabel(char *word, struct label* labels, int curr_label, int offset)
{
    assert(word != NULL);
    assert(labels != NULL);
    
    char *islabel = strchr(word, ':');
            
    if (islabel)                                                 
    {                                                               
        labels[curr_label].offset = offset;                                          // Смещение относительно начала 
        labels[curr_label].name = (char *)calloc(islabel - word + 1, sizeof(char));  // Освобождаем память под имя указатель
        
        strncpy(labels[curr_label].name, word, islabel - word);                     
        
        return 1;
    }
    else
        return 0;
}

//-----------------------------------------------------------------------------------------------------------------------

void ReadArg(char **rip, int type, char *argument, struct label *labels, int count_of_labels)
{
    assert(rip != NULL);
    assert(labels != NULL);    

    switch(type)
    {
        case REGISTER:
        {
            char * reg = (char *)calloc(4, sizeof(char));
            sscanf(argument, "%3s", reg);
                                            
            #define DEF_REG(enum_name, name, number)    \
            if (!strcmp(reg, #name))                    \
                **rip = REG_ ## enum_name;              \
            
            #include "REGISTERS.H"
            #undef DEF_REG
            
            *rip += sizeof(char);                        
            free(reg);
            break;
        }
        case VALUE:
        {
            double arg = 0;
            sscanf(argument, "%lg", &arg);
            
            *(double *)(*rip) = arg;
            *rip += sizeof(double); 
            break;
        }
        case MULTI:                       //TODO: char size
        {
            int offset_reg = 0;
            int offset_val = 0;
            
            if (argument[0] == '[' )                              // Обращение к оперативке
            {
                if (argument[strlen(argument) - 1] == ']')
                {
                    **rip = **rip | 04;
                    offset_reg = 1;
                    offset_val = 1;
                }
                else
                    printf("THERE IS NO '[' after ']'\n");        
            }
            
            if (argument[offset_reg] == 'r')         
            {
                **rip = **rip | 02;                    
                offset_val = offset_reg + 3;
            }
            
            if (isdigit(argument[offset_val]) || argument[offset_val] == '-' || argument[offset_val] == '+') 
                **rip = **rip | 01;
                
            *rip += sizeof(char);
            
            if (argument[offset_reg] == 'r')
                ReadArg(rip, REGISTER, argument + offset_reg, labels, count_of_labels);
            if (isdigit(argument[offset_val]) || argument[offset_val] == '-' || argument[offset_val] == '+') 
                ReadArg(rip, VALUE, argument + offset_val, labels, count_of_labels);
            break;
        }
        case LABEL:
        {
            for (int i = 0; i < count_of_labels; i++)
            {
                if (labels[i].offset != -1 && !strcmp(labels[i].name, argument))
                {
                    *(short *)*rip = labels[i].offset;
                    break;
                }
            }
            *rip += sizeof(short);
            break;
        }
        case NONE:
            break;
        default:
            printf("WRONG ARGUMENT TYPE IN ASSEMBLER (ReadArg function)\n");
            break;                                                               
    }
}

//-----------------------------------------------------------------------------------------------------------------------

int Parsing(struct Text_t *assm, char ** words, int *count_of_labels)
{
    assert(assm  != NULL);
    assert(words != NULL);
    
    int curr_word  = 0;
    char *check = NULL;
        
    for (int i = 0; i < assm->countoflines; i++) 
    {  
        check = strtok(assm->lines[i].pointer, " \t\r");
        
        while (check != NULL)
        {
            words[curr_word] = check;
            
            if (words[curr_word][0] == '/')
            {   
                curr_word++;
                break;
            }
            if (strchr(words[curr_word], ':'))
                (*count_of_labels)++;
                
            curr_word++;
            
            check = strtok(NULL, " \t\r");
        }
    }
    return curr_word;
}

//-----------------------------------------------------------------------------------------------------------------------

#define DEF_CMD( enum_name, assm_name, enum_code, argument_type, code )              \
    else if (!strcmp(words[curr_word], #assm_name))                                  \
    {                                                                                \
        sprintf(rip, "%c", CMD_ ## enum_name );                                      \
        rip++;                                                                       \
        curr_word++;                                                                 \
                                                                                     \
        ReadArg(&rip, argument_type, words[curr_word], labels, count_of_labels);     \
        if (argument_type != NONE)                                                   \
            curr_word++;                                                             \
    }                                                                               

//-----------------------------------------------------------------------------------------------------------------------

char * Filling(char * bytecode_line, char ** words, struct label *labels, int count_of_words, int count_of_labels)
{
    assert(bytecode_line != NULL);
    assert(words != NULL);
    assert(labels != NULL);
    
    int curr_label = 0;
    char * rip = bytecode_line;
    
    sprintf(rip, "UT%c", VER);
    rip += 3;
    
    int curr_word = 0;
    
    while (curr_word < count_of_words) 
    {   
        if (SetLabel(words[curr_word], labels, curr_label, (int)(rip - bytecode_line)))
        {
            curr_label++;
            curr_word++;
            continue;                                             
        }
        if (words[curr_word][0] == '/')
        {
            curr_word++;
            continue;
        }
                
        if (0)
            printf("No way...\n");
            
        #include "COMMANDS.H"
        
        else
        {
            printf("WRONG SYNTAX ASSEMBLER\n");
            printf("UNKNOWN COMMAND : %s, %i word\n", words[curr_word], curr_word);
            curr_word++;                                                
        }
    }
    return rip;
}

#undef DEF_CMD

//-----------------------------------------------------------------------------------------------------------------------

void Assembler(const char* assm_file, const char* bytecode_file)
{
    assert(assm_file     != NULL);
    assert(bytecode_file != NULL);
    
    struct Text_t assm = {0};
    TextConstruct(&assm, assm_file);                                  
    
    int count_of_labels = 0;
    char ** words = (char **)calloc(assm.countoflines * 10, sizeof(char*));            // Количество слов, на которые надо разбивать
    int count_of_words = Parsing(&assm, words, &count_of_labels);
    
    struct label *labels = (struct label *)calloc(count_of_labels, sizeof(struct label));
    
    for (int i = 0; i < count_of_labels; i++)                                          // Заполняем массив указателей (изначально -1)
        labels[i].offset = -1;                                                 
        
    char *  bytecode_line = (char *)calloc(count_of_words * 10 + 3, sizeof(char));                      // Массив, куда записывается машинный код
    
    char * rip = Filling(bytecode_line, words, labels, count_of_words, count_of_labels);
           rip = Filling(bytecode_line, words, labels, count_of_words, count_of_labels);
    
    FILE *bytecode = fopen(bytecode_file, "wb");
    assert(bytecode != NULL);
    fwrite(bytecode_line, rip - bytecode_line, sizeof(char), bytecode);
    fclose(bytecode);
    
    for (int i = 0; i < count_of_labels; i++)
    {
        if (labels[i].name != NULL)
            free(labels[i].name);
    }
    free(labels); 
    free(bytecode_line);
    free(words);
    TextDeconstruct(&assm);
}

//-----------------------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    Assembler(argv[1], argv[2]);
    
    return 0;
}