#pragma once
#include <stdio.h>
#include "src/str.h"

const int ram_size = 500000;

const int reg_count = 9;

const int REG_ARG = 2;
const int IMMED_ARG = 1; 
const int RAM_ARG = 4;

//#define WIN

struct label
{
    char *name;
    int offset;
};

enum arg_type 
{
    NONE = 0,
    REGISTER = 1, 
    LABEL = 2, 
    MULTI = 3,   // For push
    VALUE = 8,
};
