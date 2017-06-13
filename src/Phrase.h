#ifndef PHRASE_H
#define PHRASE_H

#include <vector>
#include <string>

struct Phrase {
    char** argArray = nullptr;
    int argSize = -1;
    std::string inFile = "";
    std::string outFile = "";
    bool appendModeOn = false;
};

#endif