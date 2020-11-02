#define DEF_CMD( enum_name, assm_name, enum_code, argument_type, code ) \
    CMD_ ## enum_name = enum_code,

enum Commands
{
    #include "COMMANDS.H"
};

#undef DEF_CMD