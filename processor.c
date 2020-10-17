#include <stdio.h>
#include "assembler.h"
#include "src/stack.h"

int main(int argc, char **argv)
{
    const char *bytecode_file = "bytecode.h";
    if (argc > 1)
        bytecode_file = argv[1];
    
    struct Stack_t* stack = NULL;
    StackConstruct(&stack, 100);
    
    FILE *bytecode = fopen(bytecode_file, "rb");
    
    long long command = 0;
    fread(&command, sizeof(long long), 1, bytecode);
    while(command != EOF)
    {
        double tmp1 = 0;
        double tmp2 = 0;
        long long tmp_longlong = 0; 
        switch (command)
        {
        case PUSH_ASS :
            fread(&tmp_longlong, sizeof(long long), 1, bytecode);
            tmp1 = ((double)tmp_longlong) / 256;
            StackPush(&stack, tmp1);
            break;
        case ADD_ASS :
            StackPop(&stack, &tmp2);
            StackPop(&stack, &tmp1);
            tmp1 += tmp2;
            StackPush(&stack, tmp1); 
            break;
        case SUB_ASS :
            StackPop(&stack, &tmp2);
            StackPop(&stack, &tmp1);
            tmp1 -= tmp2;
            StackPush(&stack, tmp1); 
            break;
        case MUL_ASS :
            StackPop(&stack, &tmp2);
            StackPop(&stack, &tmp1);
            tmp1 *= tmp2;
            StackPush(&stack, tmp1); 
            break;
        case DIV_ASS :
            StackPop(&stack, &tmp2);
            StackPop(&stack, &tmp1);
            tmp1 /= tmp2;
            StackPush(&stack, tmp1); 
            break;
        case OUT_ASS :
            StackPop(&stack, &tmp1);
            printf("VALUE = " ELEM "\n", tmp1);
            break;
        case HET_ASS :
            return 0;
            break;
        default :
            printf("BAN\n");
            break;
        }
        fread(&command, sizeof(long long), 1, bytecode);
    }
}