#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "assembler.h"
#include "src/stack.h"
#include "src/str.h"

//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------GRAPHICS-----------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------

#ifdef WIN

#include "C:\Users\anton\Desktop\TXLib\TXLib.h"

void Draw(int high, int width, double  *RAM)
{
    int memory_pointer = ram_size / 2;

    for (int y = 0; y < high && memory_pointer < ram_size; y++)
    {
        for (int x = 0; x < width && memory_pointer < ram_size; x++)
       {
            unsigned int color_tmp = (unsigned int)RAM[memory_pointer++];
            txSetPixel(x, y, RGB(color_tmp % 256, (color_tmp >> 8) % 256, (color_tmp >> 16) % 256));
        }    
    }
}

#else

#include <SFML/Graphics.hpp>
#include <unistd.h>

void Draw(sf::RenderWindow* window, int high, int width, double *RAM)
{
    assert(window != NULL);

    if (!window->isOpen())
    {
        printf("Sorry, processor can't draw the window cos it's not open, use \"crtw\" before\n");
        return;
    }

    sf::RectangleShape rectangle;

    rectangle.setSize(sf::Vector2f(1, 1));

    int memory_pointer = ram_size / 2;

    for (int y = 0; y < high && memory_pointer < ram_size; y++)
    {
        for (int x = 0; x < width && memory_pointer < ram_size; x++)
       {
            unsigned int color_tmp = (unsigned int)RAM[memory_pointer++];
            rectangle.setFillColor(sf::Color(color_tmp % 256, (color_tmp >> 8) % 256, (color_tmp >> 16) % 256) );
            rectangle.setPosition(x, y);
            window->draw(rectangle);
        }    
    }
    
    window->display();
}

void Show(sf::RenderWindow* window)
{
    assert(window != NULL);

    if (!window->isOpen())
    {
        printf("Sorry, processor can't show the window cos it's not open, use \"crtw\" before\n");
        return;
    }

    sf::Event event;
    while (window->waitEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
    }
}

#endif

//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------PROCESSOR----------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------

#define VER 8

int BytecodeVerify(char *rip)
{
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

#define DEF_CMD( enum_name, assm_name, enum_code, argument_type, code )              \
    case enum_code :                                                                 \
        code; break;

void Execute(const char *bytecode_file)
{
    assert(bytecode_file != NULL);
    
    int width = sqrt(ram_size / 2);
    int high  = sqrt(ram_size / 2);
    
    
    #ifdef WIN
        
    #else
    
    sf::RenderWindow window(sf::VideoMode(1, 1), "SFML window");
    window.close();
    
    #endif
    
    struct Stack_t* stack = NULL;
    StackConstruct(&stack, 1000);
    
    struct Stack_t* func_stack = NULL;
    StackConstruct(&func_stack, 100);
    double *registers = (double *)calloc(reg_count + 1, sizeof(double));
    const char ** registers_name = (const char **)calloc(reg_count + 1, sizeof(char *));
    
    #define DEF_REG(enum_name, name, number)\
        registers_name[number] = #name;
    #include "REGISTERS.H"
    #undef DEF_REG
    
    FILE *bytecode = fopen(bytecode_file, "rb");
    assert(bytecode != NULL);
    
    double *RAM = (double *)calloc(ram_size + 1000, sizeof(double));
    
    int count_bytes = FileSize(bytecode);
    char *buffer = FileToBuffer(bytecode);
    char *rip = buffer;
    fclose(bytecode);
    
    if (!BytecodeVerify(rip))
    {
        printf("WRONG VERSION OR SIGNATURE\n");
        return;
    }
    rip += 3;
        
    while (rip - buffer < count_bytes)
    {
        unsigned char command = *rip;
        rip++;
        switch(command)
        {
            #include "COMMANDS.H"
            default :
                printf("BAN IN PROCESSOR, rip = %i\n", (int)(rip - buffer));
                printf("UNKNOWN COMMAND : %i\n", command);
                break;
        }
    }
    
    StackDestruct(func_stack);
    StackDestruct(stack);
    free(buffer);
    free(RAM);
    free(registers_name);
    free(registers);
    
}

int main(int /*argc*/, char **argv)
{    
    Execute(argv[1]);
}