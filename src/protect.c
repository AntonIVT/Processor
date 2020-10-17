#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "str.h"
#include "stack.h"

#define MAX_SIZE_T  ( ~(size_t)0 ) 

//-----------------------------------------------------------------------------------------------------------------------------------

Stack_code StackFindDestruct()
{
    FILE * file = fopen(FILE_NAME, "rb");
    
    char *buffer = FileToBuffer(file);
    
    if (CountOfStr(buffer, "StackConstruct") != CountOfStr(buffer, "StackDestruct"))
        return STACK_ERROR;
    else
        return STACK_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------

static unsigned long long StackHashData(struct Stack_t *thou)
{      
    assert(thou != NULL);
    
    unsigned long long s1 = 1;
    unsigned long long s2 = 0;
    char *pointer = (char *)thou->data;
    
    for (int i = 0; i < (thou->size) * sizeof(StElem_t); i++)
    {
        s1 = (s1 + 53*((unsigned char)*pointer)) % 65521;
        s2 = (s2 + s1) % 65521;
        pointer++;
    }
    return ( s2 << 16 ) + s1;
    
}

//-------------------------------------------------------------------------------------------------------------------------------

static unsigned long long StackHashStruct(struct Stack_t *thou)
{
    assert(thou != NULL);
    
    unsigned long long s = 0;
    int simple = 1;
    
    char * pointer = (char *)thou;
    for (int i = 0; i < (sizeof(unsigned long long) + 2 * sizeof(size_t)); i++)
    {
        s += ((unsigned char)*pointer) * simple;
        simple *= 227;
        pointer++;
    }
    
    return s;
}

//-------------------------------------------------------------------------------------------------------------------------------

void StackLog(struct Stack_t *thou, Func_name func, Stack_code_error err, int line)
{   
    assert(thou != NULL);
    
    const char * func_name = NULL;
    
    switch (func) 
    {
    case CONSTRUCT :
        func_name = "StackConstruct";
        break;
    case RESIZEUP : 
        func_name = "StackResizeUp";
        break;
    case RESIZEDOWN : 
        func_name = "StackResizeDown";
        break;
    case PUSH : 
        func_name = "StackPush";
        break;
    case BACK : 
        func_name = "StackBack";
        break;
    case POP : 
        func_name = "StackPop";
        break;
    default:
        func_name = "Wrong name of the functions :'(";
        break;
    }
    
    FILE * log = fopen("log.txt", "ab");
    
    fprintf(log, "\n\n\n"
                 "##################################################### \n"
                 "##################################################### \n" 
                 "##################### STACK LOG ##################### \n"
                 "##################################################### \n"
                 "##################################################### \n\n\n");
    
    fprintf(log, "########## FUNCTION %s CRASHED  \n", func_name);
    
    fprintf(log, "########## LINE IN FUNCTION = %i\n", line);
    
    fprintf(log, "########## STACK POINTER    = %p\n\n", thou);
    
    if (err == WRONG_POINTER)
    {
        fprintf(log, "########## Thats all, wrong pointer, nothing can do :(\n\n");
        return;
    }
    
    fprintf(log, "########## CANARY LEFT  = %X  \n", thou->left_canary);
    fprintf(log, "########## CANARY RIGHT = %X  \n\n", *((unsigned long long *)(thou->data + thou->capacity)));
    
    fprintf(log, "########## HASH OF STRUCTURE          = %llu \n", thou->hash_struct);
    if (err == WRONG_HASH_STRUCT)
    fprintf(log, "########## EXPECTED HASH OF STRUCTURE = %llu \n", StackHashStruct(thou));
    fprintf(log, "########## HASH OF DATA               = %llu \n", thou->data);
    if (err == WRONG_HASH_DATA)
    fprintf(log, "########## EXPECTED HASH OF DATA      = %llu \n", StackHashData(thou));
    fprintf(log, "\n");
    
    fprintf(log, "########## CAPACITY = %zu \n", thou->capacity);
    fprintf(log, "########## SIZE     = %zu \n\n", thou->size);
    
    if (err == WRONG_HASH_STRUCT || err == STACK_OVERFLOW)
    {
        fprintf(log, "########## Thats all, control data is dump, nothing can do :(\n\n");
        return;
    }
    
    fprintf(log, "########## STACK INSIDE : \n");
    for (int i = 0; i < thou->size && i < 100; i++)
        fprintf(log, "### data[%3i] = "ELEM"\n", i, thou->data[i]);
        
    for (int i = thou->size; i < thou->capacity && i < 100; i++)
        fprintf(log, "### data[%3i] = %X\n", i, thou->data[i]);
    
    fclose(log);
    
    return;
}

//-------------------------------------------------------------------------------------------------------------------------------

void StackPrintProblem(Stack_code_error err)
{
    switch(err)
    {
    case NOT_ENOUGH_DESTRUCT_IN_FILE :
        printf("\n"
               " ### COUNT OF CONSTRUCT != COUNT OF DESTRUCT ### \n\n");
        break;
    case NOT_ENOUGH_MEMORY_TO_CONSTRUCT :
        printf("\n"                                                                                              
               " ### NOT ENOUGH MEMORY FOR STACK ### \n"                                                                          
               " ###  PROBLEM IN STACKCONSTRUCT  ### \n\n");
        break;
    case NOT_ENOUGH_MEMORY_TO_RESIZE :
        printf("\n"
               " ### NOT ENOUGH MEMORY FOR STACK ### \n"                                                                          
               " ###   PROBLEM IN STACKRESIZE    ### \n\n");
        break;
    case WRONG_POINTER : 
        printf("\n"
               " ### WRONG POINTER TO STACK      ### \n"
               " ### ALL INFORMATION IN log.txt  ### \n\n");
        break;
    case STACK_OVERFLOW : 
        printf("\n"
               " ###     STACK OVERFLOW          ### \n"
               " ### ALL INFORMATION IN log.txt  ### \n\n");
        break;                                    
    case LEFT_CANARY_DEAD :                         
        printf("\n"                                     
               " ###    LEFT CANARY IS DEAD      ### \n"
               " ### ALL INFORMATION IN log.txt  ### \n\n");
        break; 
    case RIGHT_CANARY_DEAD :
        printf("\n"
               " ###    RIGHT CANARY IS DEAD     ### \n"
               " ### ALL INFORMATION IN log.txt  ### \n\n");
        break; 
    case WRONG_HASH_STRUCT : 
        printf("\n"
               " ###  WRONG HASH_STRUCT_SUMMARY  ### \n"
               " ### ALL INFORMATION IN log.txt  ### \n\n");
        break; 
    case WRONG_HASH_DATA : 
        printf("\n"
               " ###   WRONG HASH_DATA_SUMMARY   ### \n"
               " ### ALL INFORMATION IN log.txt  ### \n\n");
        break; 
    default :
        printf("\n"
               " ###     WRONG ERROR TYPE    ### \n\n");
        break;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------

int IsPointerOK(void *stk) 
{   
    assert(stk != NULL);
    if (stk > (void *)4096)
        return 1;
    else
        return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------

Stack_code StackVerify(struct Stack_t *thou, Hash_info hash_mode, Func_name func, int line) 
{   
    /// Checking wrong pointer
    if (!IsPointerOK(thou))
    {
        StackPrintProblem(WRONG_POINTER);
        StackLog(thou, func, WRONG_POINTER, line);
        return STACK_ERROR; 
    }
    if (hash_mode == HASH_CHECK)
    {
        /// Checking hash of structure
        if (thou->hash_struct != StackHashStruct(thou))
        {
            StackPrintProblem(WRONG_HASH_STRUCT);
            StackLog(thou, func, WRONG_HASH_STRUCT, line);
            return STACK_ERROR;
        }
        /// Checking hash of data
        if (thou->hash_data   != StackHashData(thou))
        {
            StackPrintProblem(WRONG_HASH_DATA);
            StackLog(thou, func, WRONG_HASH_DATA, line);
            return STACK_ERROR;
        }
    }
    /// Checking if size > capacity
    if (thou->size > thou->capacity)           
    {
        StackPrintProblem(STACK_OVERFLOW);
        StackLog(thou, func, STACK_OVERFLOW, line);
        return STACK_ERROR;
    }    
    /// Checking left canary
    if (thou->left_canary != CANARY)  
    {
        StackPrintProblem(LEFT_CANARY_DEAD);
        StackLog(thou, func, LEFT_CANARY_DEAD, line);
        return STACK_ERROR;
    }   
    /// Checking right canary
    if (*((unsigned long long *)(thou->data + thou->capacity)) != CANARY) 
    {
        StackPrintProblem(RIGHT_CANARY_DEAD);
        StackLog(thou, func, RIGHT_CANARY_DEAD, line);
        return STACK_ERROR;
    }
    /// Updationg hash
    if (hash_mode == HASH_UPDATE)
    {
        thou->hash_struct = StackHashStruct(thou);
        thou->hash_data   = StackHashData(thou);
    }    
    
    return STACK_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------

int IsCapacityOverflow(size_t value)
{   
    if (value > MAX_SIZE_T / (4 * sizeof(StElem_t)))
        return 1;
    else
        return 0;
}
