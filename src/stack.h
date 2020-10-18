#pragma once

/**
    ### FAQ INFORMATION ###
    
    First of all :
  - Read CUSTOMIZATION DATA                                         //list
  - Read functions documentation
  - With compilation add src/*.c (check Makefile for example)
    This stack - is a pointer. Example : struct Stack_t *stk = NULL;
    Stack has a dynamic memory.
    
\par Functions:                                                    //paragraph
        StackConstruct();
        StackDestruct();
        StackPush();
        StackPop();
        StackBack();
*/
/**
    ### CUSTOMIZATION DATA ###
    FILE_NAME - File that include stack.h need only for Construct-Destruct checker
    CHECK_ON  - Construct-Destruct checker (works only if CHECK_ON),
                to switch it off change to #define CHECK_OFF (you'll need it if you use a lot of files)
    Elem      - Need to output different types of the stack elements
    StElem_t  - Type of the stack element
*/
//---------------------------------+ 
#define FILE_NAME "main.c"             
#define CHECK_OFF                      
#define ELEM "%lg"                    
typedef double StElem_t;            
//---------------------------------+ 
 
#include <stdlib.h>

//-------------------------------------------------------------------------------------------------------------------------------

#define POISON 0xBAD       
#define CANARY 0xABADF00D  

/**
    ### TYPEDEF INFORMATION ###
    Stack_code - error codes. If function is OK returns STACK_OK, else STACK_ERROR
    another    - system information
*/
typedef enum Stack_code_error_en { STACK_OVERFLOW, WRONG_POINTER,                                    
                                   NOT_ENOUGH_MEMORY_TO_RESIZE, NOT_ENOUGH_MEMORY_TO_CONSTRUCT,       
                                   LEFT_CANARY_DEAD, RIGHT_CANARY_DEAD,                               
                                   WRONG_HASH_STRUCT, WRONG_HASH_DATA,                                
                                   NOT_ENOUGH_DESTRUCT_IN_FILE } Stack_code_error;                    
                                                                                                    
typedef enum Stack_code_en { STACK_OK, STACK_ERROR } Stack_code;                                   
                                                                                                    
typedef enum Hash_info_en { HASH_UPDATE, HASH_CHECK } Hash_info;       

//-------------------------------------------------------------------------------------------------------------------------------

/*!
    ### STUCTURE STACK INFORMATION ###
    size     - size of the stack (count of elements)
    capacity - capacity of the stack. It changes dynamically
    another  - system information
*/
struct Stack_t
{
    unsigned long long left_canary;  
    size_t size;                     
    size_t capacity;                 
    unsigned long long hash_struct;  
    unsigned long long hash_data;    
    StElem_t data[];                
};

//-------------------------------------------------------------------------------------------------------------------------------

/**
    \brief ### StackConstruct function ### 
    \param[in] thou     - pointer to stack that you want to construct
    \param[in] capacity - initial capacity of the stack
    \return    stack code (STACK_OK/STACK_ERROR) 
*/
Stack_code StackConstruct(struct Stack_t **thou, size_t capacity);

//-------------------------------------------------------------------------------------------------------------------------------

/**
    \brief ### StackDestruct function ### 
    \param[in] thou     - stack that you want to destruct
    \return    stack code (STACK_OK/STACK_ERROR) 
*/
Stack_code StackDestruct(struct Stack_t *thou);

//-------------------------------------------------------------------------------------------------------------------------------

Stack_code StackResizeUp(struct Stack_t **thou);

//-------------------------------------------------------------------------------------------------------------------------------

Stack_code StackResizeDown(struct Stack_t **thou);

//-------------------------------------------------------------------------------------------------------------------------------

/**
    \brief ### StackPush function ### 
    \param[in] thou  - pointer to stack
    \param[in] value -  value of new stack element
    \return    stack code (STACK_OK/STACK_ERROR) 
*/
Stack_code StackPush(struct Stack_t **thou, StElem_t value);

//-------------------------------------------------------------------------------------------------------------------------------

/**
    \brief ### StackPop function ### 
    \param[in] thou  - pointer to stack
    \param[in] value - pointer to value, where will be saved last element of the stack
    \return    stack code (STACK_OK/STACK_ERROR) 
*/
Stack_code StackPop(struct Stack_t **thou, StElem_t *value);

//-------------------------------------------------------------------------------------------------------------------------------

/**
    \brief ### StackBack function ### 
    \param[in] thou  - pointer to stack
    \param[in] value - pointer to value, where will be saved last element of the stack
    \return    stack code (STACK_OK/STACK_ERROR) 
*/
Stack_code StackBack(struct Stack_t *thou, StElem_t *value);