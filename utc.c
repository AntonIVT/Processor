#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int FormatVerify(const char *file_name, const char *format_type)
{
    char *check = strstr(file_name, format_type);
    if (check == NULL)
        return 0;
    return !strcmp(check, format_type);
}

int main(int argc, char **argv)
{
    const char *input_file  = NULL;
    const char *output_file = NULL;   
    char listing = 0;                   // -lst == будет произведен листинг байткода
    char assembler = 0;                  //1 == assembler, 0 == nothing, -1 == disassembler
    
    char is_input_file_named  = 0;
    char is_output_file_named = 0;
    
    for (int i = 1; i < argc && i <= 4; i++)
    {
        if (argv[i][0] == '-')
        {
            if (!strcmp(argv[i] + 1, "a"))
                assembler = 1;
            else if (!strcmp(argv[i] + 1, "d"))
                assembler = -1;
            else if (!strcmp(argv[i] + 1, "lst"))
                listing = 1;
            else
            {
                printf("ERROR\n"
                       "Wrong compailer syntax, unknown flag : %s\n", argv[i]);
                abort();
            }
            
        }
        else if (!is_input_file_named)
        {
            input_file = argv[i];
            is_input_file_named = 1;
        }
        else if (!is_output_file_named)
        {
            output_file = argv[i];
            is_output_file_named = 1;
        }
        else
        {
            printf("ERROR\n"
                   "Wrong compailer syntax, extra argument : %s\n", argv[i]);
            abort();
        }
    }
    
    if (!is_input_file_named)
    {
        printf("ERROR\n"
               "Input file doesn't named\n");
        abort();
    }
    
    char *command = (char *)calloc(30, sizeof(char)); 
    
    if (assembler == 1) //ass
    {
        if (!FormatVerify(input_file, ".assm")) // Verify input file
        {
            printf("ERROR\n"
                   "Wrong input file format\n");
            abort();
        }
        
        if (!is_output_file_named)                  
            output_file = "bytecode.code";
        else
        {
            if (!FormatVerify(output_file, ".code"))
            {
                printf("ERROR\n"
                       "Wrong output file format\n");
                abort();
            }
        }
            
        strcpy(command, "assembler ");
        strcat(command, input_file);
        strcat(command, " ");
        strcat(command, output_file);
        
        system(command);
        
        if (listing == 1)
        {
            input_file = output_file;
            char *out_file = (char *)calloc(30, sizeof(char));  
            
            strcpy(out_file, input_file);
            char *tmp = strstr(out_file, ".");
            strcpy(tmp, ".lst");
            
            memset(command, 0, 30);
            strcpy(command, "listing ");
            strcat(command, input_file);
            strcat(command, " ");
            strcat(command, out_file);
            free(out_file);
            
            system(command); 
        }
    }
    else if (assembler == -1) //disass
    {
        if (!FormatVerify(input_file, ".code")) // Verify input file
        {
            printf("ERROR\n"
                   "Wrong input file format\n");
            abort();
        }
        
        if (!is_output_file_named)       
            output_file = "disassemblered.assm";
        else
        {
            if (!FormatVerify(output_file, ".assm"))
            {
                printf("ERROR\n"
                       "Wrong output file format\n");
                abort();
            }
        }
        strcpy(command, "disassembler ");
        strcat(command, input_file);
        strcat(command, " ");
        strcat(command, output_file);
        
        system(command); 
        
    }
    else if (assembler == 0 && listing == 1) 
    {
        if (!FormatVerify(input_file, ".code")) 
        {
            printf("ERROR\n"
                   "Wrong input file format\n");
            abort();
        }
        
        if (!is_output_file_named)       
            output_file = "listing.lst";
        else
        {
            if (!FormatVerify(output_file, ".lst"))
            {
                printf("ERROR\n"
                       "Wrong output file format\n");
                abort();
            }
        }
        strcpy(command, "listing ");
        strcat(command, input_file);
        strcat(command, " ");
        strcat(command, output_file);
        
        system(command); 
    }
    else
    {
        printf("ERROR\n"
               "what you did, lol?\n");
        abort();
    }
    free(command);
}
