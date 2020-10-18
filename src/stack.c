#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "protect.h" 
#include "stack.h"
  
//-------------------------------------------------------------------------------------------------------------------------------
  
Stack_code StackConstruct(struct Stack_t **thou, size_t capacity)
{   
    
    assert(thou  != NULL);
    
    #ifdef CHECK_ON
    if (StackFindDestruct() == STACK_ERROR)
    {
        StackPrintProblem(NOT_ENOUGH_DESTRUCT_IN_FILE);
        return STACK_ERROR;
    }
    #endif
    
    if (IsCapacityOverflow(capacity))
    {
        StackPrintProblem(NOT_ENOUGH_MEMORY_TO_CONSTRUCT);
        return STACK_ERROR;
    }
    
    *thou = (struct Stack_t *)calloc(1, sizeof(struct Stack_t) + capacity * sizeof(StElem_t) + sizeof(unsigned long long)); 
                                                              
    if (*thou == NULL)
    {                                                    
        StackPrintProblem(NOT_ENOUGH_MEMORY_TO_CONSTRUCT);    
        return STACK_ERROR;                                                                                                                                      
    }
              
    (*thou)->left_canary                                = CANARY; 
    *((unsigned long long *)((*thou)->data + capacity)) = CANARY;                         
                                                             
    for (int i = 0; i < capacity; i++)
        (*thou)->data[i] = (StElem_t)POISON;
        
    (*thou)->capacity = capacity;
    (*thou)->size        = 0;
    (*thou)->hash_data   = 0;
    (*thou)->hash_struct = 0;
    
    if (StackVerify(*thou, HASH_UPDATE, __FUNCTION__, __LINE__) == STACK_OK)
        return STACK_OK;
    else
        return STACK_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------------------

Stack_code StackDestruct(struct Stack_t *thou)
{
    assert(thou != NULL);
    
    if (StackVerify(thou, HASH_CHECK, __FUNCTION__, __LINE__) == STACK_ERROR)
        return STACK_ERROR;
    
    free(thou);
    
    return STACK_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------

Stack_code StackResizeUp(struct Stack_t **thou)
{   
    assert(thou  != NULL);
    assert(*thou != NULL);
    
    if (StackVerify(*thou, HASH_CHECK, __FUNCTION__, __LINE__) == STACK_ERROR)
        return STACK_ERROR;
    
    size_t tmp_size = ((*thou)->capacity > 0 ? (*thou)->capacity : 1);
    
    if (IsCapacityOverflow((*thou)->capacity * 2))
    {
        StackPrintProblem(NOT_ENOUGH_MEMORY_TO_RESIZE);
        return STACK_ERROR;
    }
    
    struct Stack_t *check = (struct Stack_t *)realloc(*thou, sizeof(struct Stack_t) + (tmp_size * 2 * sizeof(StElem_t)) + sizeof(unsigned long long));
    
    if (check != NULL)
    {   
        *thou = check;
        (*thou)->capacity = tmp_size * 2;
        
        for (int i = (*thou)->size; i < (*thou)->capacity; i++)
            (*thou)->data[i] = (StElem_t)POISON;
        
        *((unsigned long long *)((*thou)->data + (*thou)->capacity)) = CANARY;
            
        if (StackVerify(*thou, HASH_UPDATE, __FUNCTION__, __LINE__) == STACK_OK)
            return STACK_OK;
        else
            return STACK_ERROR;
    }
    else
    {   
        StackPrintProblem(NOT_ENOUGH_MEMORY_TO_RESIZE);
        return STACK_ERROR;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------

Stack_code StackResizeDown(struct Stack_t **thou)
{
    assert(thou  != NULL);
    assert(*thou != NULL);
    
    if (StackVerify(*thou, HASH_CHECK, __FUNCTION__, __LINE__) == STACK_ERROR)
        return STACK_ERROR;
    
    struct Stack_t *check = (struct Stack_t *)realloc(*thou, sizeof(struct Stack_t) + sizeof(StElem_t) * (((*thou)->capacity) / 2) + sizeof(unsigned long long));  
    
    if (check != NULL)
    {   
        *thou = check;
        
        (*thou)->capacity = (*thou)->capacity / 2;
        *((unsigned long long *)((*thou)->data + (*thou)->capacity)) = CANARY;
        
        if (StackVerify(*thou, HASH_UPDATE, __FUNCTION__, __LINE__) == STACK_OK)
            return STACK_OK;
        else
            return STACK_ERROR;
    }
    else
    {   
        StackPrintProblem(NOT_ENOUGH_MEMORY_TO_RESIZE);
        return STACK_ERROR;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------

Stack_code StackPush(struct Stack_t **thou, StElem_t value)
{   
    assert(thou  != NULL);
    assert(*thou != NULL);
    
    if (StackVerify(*thou, HASH_CHECK, __FUNCTION__, __LINE__) == STACK_ERROR)
        return STACK_ERROR;
        
    size_t size_tmp = (*thou)->size;                
    
    if ((*thou)->capacity == size_tmp)
    {    
        if (StackResizeUp(thou) == STACK_ERROR)             
            return STACK_ERROR;                                    
            
        (*thou)->data[size_tmp] = value;                                  
        (*thou)->size++;                                                 
                                                                          
        if (StackVerify(*thou, HASH_UPDATE, __FUNCTION__, __LINE__) == STACK_OK)  
            return STACK_OK;
        else
            return STACK_ERROR;
    }
    else if ((*thou)->capacity > size_tmp)
    {
        (*thou)->data[size_tmp] = value;
        ((*thou)->size)++;
        if (StackVerify(*thou, HASH_UPDATE, __FUNCTION__, __LINE__) == STACK_OK)
            return STACK_OK;
        else
            return STACK_ERROR;
    }
    else 
        return STACK_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------------------

Stack_code StackPop(struct Stack_t **thou, StElem_t *value)
{
    assert(thou  != NULL);
    assert(*thou != NULL);
    assert(value != NULL);
    
    if (StackVerify(*thou, HASH_CHECK, __FUNCTION__, __LINE__) == STACK_ERROR)
        return STACK_ERROR;
    
    size_t size_tmp = (*thou)->size;
    
    if (size_tmp > 0)
    {   
        *value = (*thou)->data[size_tmp - 1];
        
        (*thou)->data[size_tmp - 1] = POISON;
        ((*thou)->size)--;
        
        if (StackVerify(*thou, HASH_UPDATE, __FUNCTION__, __LINE__) == STACK_ERROR)
            return STACK_ERROR;
        
        if ((*thou)->capacity > 4 * (*thou)->size)
        {
            if (StackResizeDown(thou) == STACK_ERROR)
                return STACK_ERROR;
        }
        
        if (StackVerify(*thou, HASH_UPDATE, __FUNCTION__, __LINE__) == STACK_OK)
            return STACK_OK;
        else
            return STACK_ERROR;
    }
    else if (size_tmp == 0)
        return STACK_ERROR;
        
    return STACK_ERROR;
}

//-------------------------------------------------------------------------------------------------------------------------------

Stack_code StackBack(struct Stack_t *thou, StElem_t *value)
{
    assert(thou  != NULL);
    assert(value != NULL);
    
    if (StackVerify(thou, HASH_CHECK, __FUNCTION__, __LINE__) == STACK_ERROR)
        return STACK_ERROR;
    
    size_t size_tmp = thou->size;
    
    if (size_tmp > 0)
    {   
        *value = thou->data[size_tmp - 1];
        
        return STACK_OK;
    }
    else if (size_tmp == 0)
        return STACK_ERROR;
            
    return STACK_ERROR;
}