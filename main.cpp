#include <iostream>
#include "scanner.h"
#include <fstream>
#include <map>
#define BUFFER_SIZE 4096



void printHelp() {
    std::cout << "Usage: program [options]\n"
              << "Options:\n"
              << "  -h          Show help\n"
              << "  -s <name>   scan file specified\n"
              << "  -p <name>        scan and parse specified file\n"
              << "  -r <name>        scan parse and build IR\n";
}

void ScanFile(const std::string &fileName){
    std::ifstream file(fileName);
    char buffer[BUFFER_SIZE];
    if(!file.is_open()){
        std::cerr << "ERROR: File could not be opened " << fileName << "\n";
    }

    while(file.getline(buffer,BUFFER_SIZE)){
        std::pair<Category,std::string> token = scanLine(buffer);
            while (token.first != EOL && token.first != ERROR) {
                std::cout << "Token: <" << token.first << ", " << token.second << ">\n";
                token = scanLine(buffer); 
        }

                if (token.first == ERROR) {
            std::cerr << "Error: Invalid token encountered.\n";
            return;
        }

    }
    file.close();

    std::cout << "processiing file: " << fileName << '\n';
}   



int main(int argc, char *argv[]) {
    
    initializeTransitionTable();
    std::map<std::string, int> flagPrecedence = {
        {"-h", 1}, 
        {"-s", 2}, 
        {"-p", 3},
        {"-r", 4}  
    };

    bool showHelp =false;
    bool readS =false;
    bool readParse = false;
    bool scanBuild = false;
    std::string fileName = "";
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-h") {
            showHelp = true;
        } else if (arg == "-s") {
            if (i + 1 < argc) {
                readS = true;
                fileName = argv[++i]; 
            } else {
                std::cerr << "Error: -s flag requires a file name.\n";
                return 1;
            }
        } else if (arg == "-p") {
            if (i + 1 < argc) {
                readParse = true;
                fileName = argv[++i];
            } else {
                std::cerr << "Error: -p flag requires a file name.\n";
                return 1;
            }
        } else if (arg == "-r") {
            if (i + 1 < argc) {
                scanBuild = true;
                fileName = argv[++i];
            } else {
                std::cerr << "Error: -r flag requires a file name.\n";
                return 1;
            }
        } else {
            std::cerr << "Error: Unknown argument " << arg << "\n";
            return 1;
        }
    }

    if (showHelp) {
        printHelp();
        return 0;
    }

    if (scanBuild) {
        std::cout << "Scanning, parsing, and building IR from: " << fileName << "\n";
    } else if (readParse) {
        std::cout << "Reading and parsing: " << fileName << "\n";
    } else if (readS) {
        std::cout << "Scanning file: " << fileName << "\n";
        ScanFile(fileName);
    } else {
        std::cerr << "Error: No valid operation specified.\n";
        printHelp();
        return 1;
    }


    return 0;
}
