#include <stdio.h>  //fseek(), ftell(), fread(), fputs(), fputc().
#include <assert.h> //assert()
#include <string.h> //strchr()
#include <stdlib.h> //calloc()
#include "str.h"    //struct string

//-----------------------------------------------------------------------------------------------------------------------------------

int FileSize(FILE *file)
{
    assert(file != NULL);

	int _file_size = -1;

	fseek(file, 0, SEEK_END); //Puts pointer in file to the end
	_file_size = ftell(file); //ftell() checks how many bytes from start of file to the pointer on file

	return _file_size;
}

//-----------------------------------------------------------------------------------------------------------------------------------

char* FileToBuffer(FILE* text)
{
    assert(text   != NULL);

    int size = FileSize(text);

    fseek(text, 0, SEEK_SET); //Puts pointer in file to the start

    char *buffer = (char *)calloc(size, sizeof(char));
    assert(buffer != NULL);

    fread(buffer, sizeof(char), size, text);

    return buffer;
}
//-----------------------------------------------------------------------------------------------------------------------------------

int CountOfLinesInBuffer(char *buffer)
{
    int Count = 0; 
    
    char *begin = buffer;
    char *end   = NULL;

    while((end = strchr(begin, '\n')) != NULL){
        
        assert(begin != NULL);
        begin = end + 1;

        Count++;
    }

    return Count;
}

//-----------------------------------------------------------------------------------------------------------------------------------

struct String_t* BufferToStrings(struct Text_t *text)
{    
    assert(text != NULL);

    struct String_t *lines = (struct String_t *)calloc(text->countoflines, sizeof(struct String_t));
    assert(lines != NULL);

    char *begin = text->buffer; 
    char *end   = NULL;

    for (int i = 0; i < text->countoflines; i++){
    
        end = strchr(begin, '\n'); //find first \r after start of new line

        assert(end != NULL);

        *end = '\0'; //Change '\n' to the '\0'

        (lines + i)->pointer = begin; //Puts pointer to the previous line
        (lines + i)->length = end - begin; //Puts length of line to structure

        begin = end + 1; //New start of line
    }

    return lines;
}

void TextConstruct(struct Text_t *text, const char *filename)
{
    assert(text != NULL);
    FILE *input = fopen(filename, "rb");
    assert(input != NULL);
    
    text->textsize = FileSize(input);
    text->buffer = FileToBuffer(input);
    text->countoflines = CountOfLinesInBuffer(text->buffer);
    text->lines = BufferToStrings(text);
    
    
    fclose(input);
}

void TextDeconstruct(struct Text_t *text)
{
    assert(text != NULL);
    
    free(text->buffer);
    free(text->lines);
}

//-----------------------------------------------------------------------------------------------------------------------------------

void OutputStrings(FILE* output, struct Text_t text){
    
    assert(text.lines != NULL);

    for(int i = 0; i < text.countoflines; i++){

        fputs(text.lines[i].pointer, output);
        fputc('\n', output);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------

void OutputBuffer(FILE* output, struct Text_t text){

    assert(text.buffer != NULL);

    char *begin = text.buffer;

    for(int i = 0; i < text.countoflines; i++){

        fputs(begin, output);
        fputc('\n', output);

        begin = strchr(begin, '\0') + 1;
        assert(begin != NULL);
    }
}

int CountOfStr(char* buffer, const char* sub_str)
{
    int Count = 0; 
    
    char *begin = buffer;
    char *end   = NULL;

    while((end = strstr(begin, sub_str)) != NULL)
    {    
        assert(begin != NULL);
        begin = end + 1;

        Count++;
    }

    return Count;
}