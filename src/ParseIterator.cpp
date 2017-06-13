#include "ParseIterator.h"

ParseIterator::ParseIterator(size_t start, std::string &l) : 
    line(l), 
    pos(start),
    prevPos(start),
    nextPos(start),
    current(NONE),
    last(NONE),
    token(""),
    cutAtSpace(false)
{ 
    process();
}

std::string ParseIterator::getString() {
    return this->token;
}

ExType ParseIterator::currentExType() {
    return this->current;
}

ExType ParseIterator::lastExType() {
    return this->last;
}

bool ParseIterator::done() {
    return (pos == std::string::npos || pos >= line.size());
}

void ParseIterator::advance() {
    //Update positions
    this->prevPos = pos;
    this->pos = nextPos;
    
    //Update last exType
    this->last = current;
    
    //Update special cases (such as filenames)
    if(current == RIGHT_ARROW || current == LEFT_ARROW || current == DOUBLE_RIGHT_ARROW) {
        this->cutAtSpace = true;
    }
    
    //Process next token
    this->process();
}

void ParseIterator::operator ++() {
    this->advance();
}

//Uses last exType to properly process current token
void ParseIterator::process() {
    
    //Skips leading spaces
    while(pos < line.size() && line.at(pos) == ' ') {
        ++pos;
    }
    
    //If there's nothing after the spaces, then we are done. There is nothing to process.
    if(pos >= line.size() || pos == std::string::npos) {
        return;
    }
    
    //Otherwise, we can process the token.
    
    //There are two types of tokens: TEXT-BASED TOKENS and SYMBOLS.
    //TEXT-BASED TOKENS: commands, filenames
    //SYMBOLS: && || ; < > | ( )
    
    //Let's set up a vector of SYMBOLS that we'll search for.
    //These symbols will help us determine the ending positions of TEXT-BASED TOKENS.
    std::vector<std::string> searchSymbols = {"&&", "||", ">>", ">", "<", ";", "|", "(", ")"};
    
    //If the current token is a filename, then we should also search for a space.
    if(cutAtSpace) {
        searchSymbols.push_back(" ");
        //Since this is one-time-only, we reset cutAtSpace to false.
        this->cutAtSpace = false;
    }
    
    //We iterate through "line" character-by-character until we find a symbol
    bool found = false;
    std::string symbolFound = "";
    size_t endPos = line.size();
    
    for(size_t i = pos; i < line.size() && !found; ++i) {
        for(std::string symbol: searchSymbols) {
            if(i + symbol.size() <= line.size() && line.substr(i, symbol.size()) == symbol) {
                symbolFound = symbol;
                endPos = i;
                found = true;
                break;
            }
        }
    }
    
    //If the current token is a SYMBOL, then...
    if(found && pos == endPos) {
        
        //Update token
        this->token = symbolFound;
        
        //Determine exType of current token
        if(token == "&&" || token == "||" || token == ";") {
            this->current = CONNECTOR;
        }
        else if(token == "(") {
            this->current = OPEN_PAREN;
        }
        else if(token == ")") {
            this->current = CLOSE_PAREN;
        }
        else if(token == "<") {
            this->current = LEFT_ARROW;
        }
        else if(token == ">") {
            this->current = RIGHT_ARROW;
        }
        else if(token == ">>") {
            this->current = DOUBLE_RIGHT_ARROW;
        }
        else if(token == "|") {
            this->current = PIPE;
        }
        
        this->nextPos = pos + symbolFound.size();
        return;
    }
    //Otherwise, the current token is a TEXT-BASED TOKEN...
    else{
        std::string almostToken = line.substr(pos, endPos - pos);
        
        //Dispose of following spaces
        while(almostToken.size() > 0 && almostToken.at(almostToken.size() - 1) == ' ') {
            almostToken.pop_back();
        }
        
        //Update token
        this->token = almostToken;
        
        //Determine exType of current token
        if(last == RIGHT_ARROW || last == LEFT_ARROW || last == DOUBLE_RIGHT_ARROW) {
            this->current = FILENAME;
        }
        else {
            this->current = COMMAND;
        }
        
        //Find nextPos
        this->nextPos = endPos;

    }
    
    
}