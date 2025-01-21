#ifndef ENUMS_h
#define ENUMS_h

enum Category {
        MEMOP, LOADI, ARITHOP, OUTPUT, NOP,
    CONSTANT, REGISTER, COMMA, INTO,
    END_OF_FILE, EOL, ERROR, EQUALS  
};

enum charClass {
    REGISTER_BEGIN,
    DIGIT,
    LETTER,
    OP,
    OTHER,
};



#endif