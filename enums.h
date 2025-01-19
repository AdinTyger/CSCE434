#ifndef ENUMS
#define ENUMS

enum Category {
    MEMOP, LOADI, ANRITHOP, OUTPUT, NOP, CONSTANT, REGISTER, COMMA,INTO, EOF, EOL
};

enum charClass {
    REGISTER,
    DIGIT,
    LETTER,
    OP,
    OTHER,
};

#endif