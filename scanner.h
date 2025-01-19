/*
This is the scanner
the scanner should look at each character then return a token
the tokens are defined as <word,Lexeme>

o: 14
t: 12
l: 7
e: 6
a: 6
u: 6
s: 6
m: 5
p: 5
d: 5
n: 5
i: 5
r: 4
f: 3
c: 2
h: 2
g: 1
b: 1


LINUX EOL = "\n" WINDOWS "\r\n"
*/

#include <iostream>
#include <array>
#include <fstream>
#include <enums>

#define BUFFER_SIZE 2048



const char* keywords [10] = 
{"load","loadI","store","add","sub","mult","lshift","rshift","output","nop"};

const char* keywords[10] =
{"MEMOP","LOADL","OUTPUT","NOP","CONSTANT","REGISTER","COMMA","INTO","EOF","EOL"};

char buffer[BUFFER_SIZE];



charClass getCharClass(char c){
    if(c == 'r'){
        return REGISTER;
    }else if (c >= '0' && c <='9'){
        return DIGIT;
    }else{
        return OTHER;
    }
}


std::pair<int,int> scanLine(char* line){
    char c = line[0];

}