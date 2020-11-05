///////////////////////////////////////////////////////  DEFINES BECOUSE I'M LAZY////////////////////////////////////////////////////////


#define STACKPOP(pointer)                                \
    {                                                    \
        if (StackPop(&stack, pointer) == STACK_ERROR)    \
            printf("HEAP UNDERFLOW\n");                  \
    }

#define STACKPUSH(value)                                 \
    {                                                    \
        if (StackPush(&stack, value) == STACK_ERROR)     \
            printf("HEAP OVERFLOW\n");                   \
    }

#define ARITHMETIC(cond)                                 \
    {                                                    \
        double tmp1 = 0;                                 \
        double tmp2 = 0;                                 \
        STACKPOP(&tmp2);                                 \
        STACKPOP(&tmp1);                                 \
        STACKPUSH(tmp1 cond tmp2);                       \
    }                                                    
    
#define CONDITIONAL(cond)                                \
    {                                                    \
        double tmp1 = 0;                                 \
        double tmp2 = 0;                                 \
        STACKPOP(&tmp2);                                 \
        STACKPOP(&tmp1);                                 \
        if (tmp1 cond tmp2)                              \
            rip = buffer + *(short *)rip;                \
        else                                             \
            rip += LABEL;                                \
    }
#define FUNCTION2(cond)                                  \
    {                                                    \
        double tmp1 = 0;                                 \
        double tmp2 = 0;                                 \
        STACKPOP(&tmp2);                                 \
        STACKPOP(&tmp1);                                 \
        STACKPUSH( cond (tmp1, tmp2));                   \
    }
#define FUNCTION1(cond)                                  \
    {                                                    \
        double tmp = 0;                                  \
        STACKPOP(&tmp);                                  \
        STACKPUSH( cond(tmp) );                          \
    }
    
/////////////////////////////////////////////////////// STACK OPERATIONS ////////////////////////////////////////////////////////

DEF_CMD (PUSH, push, 1, MULTI,                                           
    {
        double tmp = 0;
        int type = *(rip++);     
        if (type & REG_ARG)
        {   
            tmp += registers[(int)*rip];   
            rip++;     
        }              
        if (type & IMMED_ARG)                       
        {                                   
            tmp += *(double *)rip;          
            rip += sizeof(double);          
        }
        
        if (type & RAM_ARG)
        {
            if (tmp >= ram_size || tmp < 0)
            {
                printf("SEGMENTATION FAULT!!!!\n");
            }
            else
                STACKPUSH(RAM[(int)tmp]);
        }
        else
            STACKPUSH(tmp);
    })

DEF_CMD (POP,   pop,   2, MULTI,                                         
    {
        double tmp = 0;
        int type = *(rip++);   
        int reg = 0;
        if (type & REG_ARG)
        {
            reg = *(rip++);
            tmp += registers[reg];
        }
        if (type & IMMED_ARG)
        {
            tmp += *(double *)rip;
            rip += sizeof(double);
        }
        if (type & RAM_ARG)
        {
            if (tmp >= ram_size || ram_size < 0)
            {
                printf("SEGMENTATION FAULT!!!!\n");
            }
            else
            {
                double tmp_pointer = 0;
                STACKPOP(&tmp_pointer);
                RAM[(int)tmp] = tmp_pointer;
            }
        }
        else
            STACKPOP(registers + reg);
    })
    
DEF_CMD (CPY,   cpy,   0xA3, NONE,
    {
        double tmp = 0;
        STACKPOP(&tmp);
        STACKPUSH(tmp);
        STACKPUSH(tmp);
    })

/////////////////////////////////////////////////////// REGISTERS OPERATIONS ////////////////////////////////////////////////////////

DEF_CMD (IN,    in,    3, REGISTER,                                         
    {
        if (*rip > reg_count || *rip <= 0)
        {
            printf("Wrong register index in bytecode file\n"
                   "RIP = %d", rip - buffer);
            rip += REGISTER;
        }
        else{
            printf("INPUT VALUE FOR REGISTER %s : " , registers_name[(int)*rip]);
            
            scanf("%lg", registers + *rip);
            rip += REGISTER;
        }
    })

/////////////////////////////////////////////////////// ARITHMETIC OPERATIONS ////////////////////////////////////////////////////////

DEF_CMD (ADD,   add,   4, NONE, ARITHMETIC(+))
DEF_CMD (SUB,   sub,   5, NONE, ARITHMETIC(-))
DEF_CMD (MUL,   mul,   6, NONE, ARITHMETIC(*))
DEF_CMD (DIV,   div,   7, NONE, ARITHMETIC(/))

DEF_CMD (IDIV, idiv,   9, NONE, 
    {
        double tmp1 = 0;
        double tmp2 = 0;
        STACKPOP(&tmp2);
        STACKPOP(&tmp1);
        STACKPUSH(( long long)tmp1 / (long long)tmp2 );
    })

DEF_CMD (POW,   pow,   8,  NONE, FUNCTION2(pow))
DEF_CMD (SIN,   sin,   10, NONE, FUNCTION1(sin))
DEF_CMD (COS,   cos,   11, NONE, FUNCTION1(cos))
DEF_CMD (ROUND, round, 12, NONE, FUNCTION1(round))

/////////////////////////////////////////////////////// SYSTEM OPERATIONS ////////////////////////////////////////////////////////

DEF_CMD (OUT,   out,   0xA1, NONE,
    {
        double tmp = 0;
        STACKPOP(&tmp);
        printf("VALUE = " ELEM "\n", tmp);
    })
    
DEF_CMD (HET,   het,   0xA2, NONE, return;)

/////////////////////////////////////////////////////// LABELS OPERATIONS ////////////////////////////////////////////////////////

DEF_CMD (JMP,   jmp,   0xC1, LABEL, 
    {
        rip = buffer + *(short *)rip;
    })
    
DEF_CMD (JA,    ja,    0xC2, LABEL, CONDITIONAL(> ))
DEF_CMD (JAE,   jae,   0xC3, LABEL, CONDITIONAL(>=))
DEF_CMD (JB,    jb,    0xC4, LABEL, CONDITIONAL(< ))
DEF_CMD (JBE,   jbe,   0xC5, LABEL, CONDITIONAL(<=))
DEF_CMD (JE,    je,    0xC6, LABEL, CONDITIONAL(==))
DEF_CMD (JNE,   jne,   0xC7, LABEL, CONDITIONAL(!=))

DEF_CMD (CALL,  call,  0xC8, LABEL, 
    {
        if (StackPush(&func_stack, (double)(rip - buffer + sizeof(short))) == STACK_ERROR)    
            printf("HEAP OVERFLOW\n");
        rip = buffer + *(short *)rip;
    })
    
DEF_CMD (RET,   ret,   0xC9, NONE,
    {
        double tmp = 0;
        if (StackPop(&func_stack, &tmp) == STACK_ERROR)    
            printf("HEAP UNFERFLOW\n");
        rip = buffer + (short)tmp;
    })
    
DEF_CMD (JM,   jm,  0x25, LABEL, 
    {
        time_t calendar_time = time(NULL);
        
        time(&calendar_time);
        
        struct tm curr_time = *(localtime(&calendar_time));
        
        if (curr_time.tm_wday == 1)
            rip = buffer + *(short *)rip;
        else
        {
            printf("Today is not a Monday :(\n");      
            rip += sizeof(short);
        }      
    })

/////////////////////////////////////////////////////// GRAPHICS OPERATIONS ////////////////////////////////////////////////////////

#ifdef WIN

DEF_CMD (CRTW, crtw,   0xA5, NONE, txCreateWindow(width, high);)

DEF_CMD (DRAW, draw,   0xA4, NONE, Draw(high, width, RAM);)

#else

DEF_CMD (DRAW, draw,   0xA4, NONE, Draw(&window, high, width, RAM);)

DEF_CMD (CRTW, crtw,   0xA5, NONE,
    {
        window.create(sf::VideoMode(width, high), "VIDEO RAM");
    })

DEF_CMD (SHOW, show, 0xA6, NONE, Show(&window);)

#endif