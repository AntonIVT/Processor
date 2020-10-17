#pragma once
///\file
/*!
    \file
    \brief Header with fuctions with strings

    Functions with struct string from strio.cpp (input and output strings) and strsort.cpp (sortings strings)
*/

#include <stdio.h> 

/*!
    \brief Structure string 

    Structure string include pointer to the line and length of this line (without \0 or \\n)
    \param pointer Pointer to the line
    \param length Length of the line without \0 or \\n
*/

struct String_t
{
    char *pointer;
    int length;
};

//-----------------------------------------------------------------------------------------------------------------------------------

struct Text_t
{
    char *buffer;
    int countoflines;
    int textsize;
    struct String_t *lines;
};

//-----------------------------------------------------------------------------------------------------------------------------------

/*!
    \brief Calculates size of the file

    Calculates size of the file in bytes and returns it. Definition in strio.cpp
    \param[in] file Pointer to the file to find the size in bytes
    \return Size of the file
*/

int FileSize(FILE *file);

//-----------------------------------------------------------------------------------------------------------------------------------

/*!
    \brief Reads all file and returns buffer

    Reads all file and calculates number of \\n, and puts it to the CountOfLines. Returns pointer to the buffer
    \param[in] text File which need to read
    \return Pointer to the first char
*/

char* FileToBuffer(FILE* text);

//-----------------------------------------------------------------------------------------------------------------------------------

/*!
    \brief Checks count of lines (\\n) in buffer

    \param[in] buffer (pointer to the line)
    \return CountOfLines Pointer to the variable
*/

int CountOfLinesInBuffer(char *buffer);

//-----------------------------------------------------------------------------------------------------------------------------------

/*!
    \brief Reads buffer and returns strings

    Gets buffer and count of lines. Changes \\n to \0 in buffer and puts pointers of strings
    \param[in] buffer Buffer which need to read
    \param[in] Count of lines
    \return Pointer to the first string
*/

struct String_t* BufferToStrings(struct Text_t *text);

//-----------------------------------------------------------------------------------------------------------------------------------

/*!
    \brief Reads file and returns data for Onegin

    Gets file in input, returns strings, Count of lines and buffer, where all \\n replaced to \0
    \param[out] PointerInMain Pointer to the first symbol of buffer
    \param[in] input input file
    \param[out] CountInMain Count of lines in buffer
    \return Pointer to the first string
*/

void TextConstruct(struct Text_t *text, const char *filename);

void TextDeconstruct(struct Text_t *text);

//-----------------------------------------------------------------------------------------------------------------------------------
/*!
    \brief Output strings to the file

    Output strings to the file output. Puts only CountOfLines strings!!!
    \param[in] output Pointer to the file
    \param[in] lines Pointer to the strings
    \param[in] CountOfLines
*/

void OutputStrings(FILE* output, struct Text_t text);

//-----------------------------------------------------------------------------------------------------------------------------------

/*!
    \brief Output buffer to the file

    Output strings to the file output. Puts only CountOfLines strings!!!
    \param[in] output Pointer to the line
    \param[in] str Pointer to the buffer
    \param[in] CountOfLines 
*/

void OutputBuffer(FILE* output, struct Text_t text);

//-----------------------------------------------------------------------------------------------------------------------------------

/*!
    \brief Compares strings in forward direct

    Returns ComparatorString with mode 1. Can be used with standard functions
*/

int ComparatorForward(const void *left, const void *right);

//-----------------------------------------------------------------------------------------------------------------------------------

/*!
    \brief Compares strings in reverse direct

    Returns ComparatorString with mode 2. Can be used with standard functions
*/

int ComparatorReverse(const void *left, const void *right);

//-----------------------------------------------------------------------------------------------------------------------------------

/*!
    \brief Compares strings in different direct

    Compares struct strings in different directs. If left < right returns negative, if left = right
    returns 0, if left > right returns positive
    \param[in] left 
    \param[in] right
    \param[in] mode of compare (1 - Forward, 2 - Reverse)
*/

int ComparatorString(struct String_t *left, struct String_t *right, // Compare mode :
                                                                   char *StartLeft,
                                                                   char *StartRight,
                                                                   char *EndLeft,
                                                                   char *EndRight,
                                                                   int step);

//-----------------------------------------------------------------------------------------------------------------------------------

/*!
    \brief Changes values of two strings

    Changes values of two strings. Uses temp string
    \param[in] left 
    \param[in] right
*/

void SwapString(struct String_t *left, struct String_t *right);

//-----------------------------------------------------------------------------------------------------------------------------------

/*!
    \brief Quick sort for strings for any comparator

    Quick sort for strings for any comparator
    \param[in] Lines Pointer to the first string
    \param[in] LeftBorder Number from which number need sort (starting from 0)
    \param[in] RightBorder Number to the which number need sort (if full massive of N elements use N - 1)
    \param[in] cmp Comparator
*/

void QuickSortString(struct String_t *Lines, int LeftBorder, int RightBorder, int (*cmp)(const void *, const void *));


int CountOfStr(char* buffer, const char* sub_str);