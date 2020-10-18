#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "assembler.h"
#include "src/stack.h"
#include "src/str.h"

#define VER 1

int main(int argc, char **argv)
{
    const char *bytecode_file = "bytecode.code";
    if (argc > 1)
        bytecode_file = argv[1];
    
    struct Stack_t* stack = NULL;
    StackConstruct(&stack, 100);
    
    FILE *bytecode = fopen(bytecode_file, "rb");
    assert(bytecode != NULL);
    
    char *buffer = FileToBuffer(bytecode);
    char *curr_ptr = buffer;
    int count_bytes = FileSize(bytecode);
    
    const char *signature = curr_ptr;
    curr_ptr += 2;
    char version = *curr_ptr;
    curr_ptr++;
    
    if (!strcmp(signature, "UT"))
    {
        printf("SORRY, WRONG SIGNATURE, CANT EXECUTE :(((\n");
        return 0;
    }
    if (version != VER)
    {
        printf("SORRY, YOUR BYTECODE HAS OLD VERSION, CANT EXECUTE :(((\n");
        return 0;
    }
    
    while (curr_ptr < buffer + count_bytes)
    {
        unsigned char command = *curr_ptr;
        curr_ptr += sizeof(char);
        double tmp1 = 0;
        double tmp2 = 0;
        switch (command)
        {
        case PUSH :
            StackPush(&stack, *(double *)curr_ptr);
            curr_ptr += sizeof(double);
            break;
        case ADD :
            StackPop(&stack, &tmp2);
            StackPop(&stack, &tmp1);
            tmp1 += tmp2;
            StackPush(&stack, tmp1); 
            break;
        case SUB :
            StackPop(&stack, &tmp2);
            StackPop(&stack, &tmp1);
            tmp1 -= tmp2;
            StackPush(&stack, tmp1); 
            break;
        case MUL :
            StackPop(&stack, &tmp2);
            StackPop(&stack, &tmp1);
            tmp1 *= tmp2;
            StackPush(&stack, tmp1); 
            break;
        case DIV :
            StackPop(&stack, &tmp2);
            StackPop(&stack, &tmp1);
            tmp1 /= tmp2;
            StackPush(&stack, tmp1); 
            break;
        case OUT :
            StackPop(&stack, &tmp1);
            printf("VALUE = " ELEM "\n", tmp1);
            break;
        case HET :
            return 0;
            break;
        default :
            printf("BAN\n");
            break;
        }
    }
}