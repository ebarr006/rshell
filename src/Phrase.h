/*
    Description: A PHRASE consists of a command and its command-line arguments and i/o redirection.
*/

#ifndef PHRASE_H
#define PHRASE_H

#include <vector>
#include <string>

struct Phrase {
    char** argArray = nullptr;  // command-line arguments (for this phrase)
    int argSize = -1;           // length of command-line arguments
    std::string inFile = "";    // input file (redirection)
    std::string outFile = "";   // output file (redirection)
    bool appendModeOn = false;  // TRUE: append redirect, FALSE: output redirect
};

#endif