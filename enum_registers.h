#define DEF_REG(enum_name, name, number) \
    REG_ ## enum_name = number,

enum Registers{
    #include "REGISTERS.H"
};

#undef DEF_REG