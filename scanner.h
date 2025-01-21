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
#include <stack>
#include <algorithm>
#include "enums.h"

std::stack<Category> stateStack;

Category transitionTable[13][5];

void initializeTransitionTable() {
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 5; ++j) {
            transitionTable[i][j] = ERROR; // Initialize all transitions to ERROR
        }
    }

    // Valid Transitions
    transitionTable[MEMOP][LETTER] = MEMOP;            // Stay in MEMOP for letters in keywords
    transitionTable[MEMOP][REGISTER_BEGIN] = REGISTER; // MEMOP to REGISTER on `r`
    transitionTable[LOADI][DIGIT] = CONSTANT;          // LOADI to CONSTANT for digits
    transitionTable[ARITHOP][OP] = ARITHOP;            // ARITHOP to ARITHOP for operators
    transitionTable[OUTPUT][LETTER] = REGISTER;        // OUTPUT to REGISTER for letters
    transitionTable[REGISTER][DIGIT] = REGISTER;       // REGISTER to REGISTER for digits
    transitionTable[MEMOP][OP] = EQUALS;               // MEMOP to EQUALS on `=`
    transitionTable[EQUALS][OP] = INTO;                // EQUALS to INTO on `>`
    transitionTable[COMMA][OTHER] = INTO;              // COMMA to INTO for other symbols
    transitionTable[END_OF_FILE][OTHER] = END_OF_FILE; // EOF remains EOF
    transitionTable[EOL][OTHER] = EOL;                 // EOL remains EOL
}


Category identifyKeyword(const std::string& word) {
    if (word == "load" || word == "store") return MEMOP;
    if (word == "loadi") return LOADI;
    if (word == "add" || word == "sub" || word == "mult" || word == "lshift" || word == "rshift") return ARITHOP;
    if (word == "output") return OUTPUT;
    if (word == "nop") return NOP;
    return ERROR;
}

charClass getCharClass(char c) {
    if (c == 'r') return REGISTER_BEGIN;        
    if (c >= '0' && c <= '9') return DIGIT;     
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return LETTER;
    if (c == ',') return OTHER;                
    if (c == '=' || c == '>') return OP;       
    return OTHER;                              
}


Category getNextState(Category currState, charClass input){
    return transitionTable[currState][input];
}



std::pair<Category, std::string> scanLine(char* buffer) {
    Category state = MEMOP;
    std::string lexeme = "";
    static int nextChar = 0; // Tracks the current position in the buffer
    char currChar = ' ';

    // Debugging: Print the buffer being processed
    std::cout << "Buffer at Start: " << buffer << std::endl;

    while (state != ERROR) {
        currChar = buffer[nextChar];

        // Handle end of line
        if (currChar == '\n') {
            nextChar++; // Advance past the newline
            if (!lexeme.empty()) {
                // Return the last token before EOL
                std::cout << "Returning Token Before EOL: <" << state << ", " << lexeme << ">" << std::endl;
                return {state, lexeme};
            }
            // Return EOL token
            std::cout << "Returning EOL Token: <" << EOL << ", \\n>" << std::endl;
            return {EOL, "\\n"};
        }

        // Handle end of buffer
        if (currChar == '\0') {
            if (!lexeme.empty() && state != ERROR) {
                std::cout << "Returning Token at End: <" << state << ", " << lexeme << ">" << std::endl;
                return {state, lexeme};
            }
            nextChar = 0; // Reset for subsequent calls
            break;
        }

        // Skip spaces
        if (currChar == ' ') {
            if (!lexeme.empty() && state != ERROR) {
                // Return the current token before processing the space
                std::cout << "Returning Token Before Space: <" << state << ", " << lexeme << ">" << std::endl;
                return {state, lexeme};
            }
            nextChar++;
            continue;
        }

        // Debugging: Show the current character and state
        std::cout << "Processing char: " << currChar << ", Lexeme: " << lexeme << ", State: " << state << std::endl;

        // Add current character to the lexeme and advance pointer
        lexeme += currChar;
        nextChar++;

        // Get the character's class
        charClass inputClass = getCharClass(currChar);

        // Debugging: Show the character class
        std::cout << "Character Class: " << inputClass << std::endl;

        // Update the state using the transition table
        state = getNextState(state, inputClass);

        // Debugging: Show the updated state
        std::cout << "Updated State: " << state << std::endl;

        // Handle token finalization for multi-character tokens like `=>`
        if (state == INTO) {
            std::cout << "Returning INTO Token: <" << state << ", " << lexeme << ">" << std::endl;
            return {state, lexeme};
        }

        // Finalize keywords like `load`, `add`, etc.
        if (state == MEMOP || state == LOADI || state == ARITHOP || state == OUTPUT || state == NOP) {
            Category keyword = identifyKeyword(lexeme);
            if (keyword != ERROR) {
                std::cout << "Returning Keyword Token: <" << keyword << ", " << lexeme << ">" << std::endl;
                return {keyword, lexeme};
            }
        }

        // Finalize register tokens like `r1`, `r2`, etc.
        if (state == REGISTER && lexeme[0] == 'r' && lexeme.size() > 1 && std::isdigit(lexeme[1])) {
            std::cout << "Returning Register Token: <" << REGISTER << ", " << lexeme << ">" << std::endl;
            return {REGISTER, lexeme};
        }

        // Finalize constant tokens (e.g., numbers like `5`, `10`)
        if (state == CONSTANT && std::all_of(lexeme.begin(), lexeme.end(), ::isdigit)) {
            std::cout << "Returning Constant Token: <" << CONSTANT << ", " << lexeme << ">" << std::endl;
            return {CONSTANT, lexeme};
        }
    }

    // If no valid token is found, return an error
    std::cout << "Returning Error Token: <ERROR, " << lexeme << ">" << std::endl;
    return {ERROR, ""};
}








