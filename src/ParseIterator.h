/********************************************************
* THE NEW AND IMPROVED PARSE ITERATOR!!!!
*********************************************************/

#ifndef PARSE_ITERATOR_H
#define PARSE_ITERATOR_H

#include <vector>
#include <iostream>
#include <string>

enum ExType { NONE, COMMAND, CONNECTOR, OPEN_PAREN, CLOSE_PAREN, LEFT_ARROW, RIGHT_ARROW, DOUBLE_RIGHT_ARROW, PIPE, FILENAME };

class ParseIterator {
    
    private:
        //basic info
        std::string &line;
        size_t pos;
        size_t prevPos;
        size_t nextPos;
        
        //information determined by process()
        ExType current;
        ExType last;
        std::string token;
        
        //helper variables for process()
        bool cutAtSpace;
    
    public:
        ParseIterator(size_t start, std::string &l);
        std::string getString();
        ExType currentExType();
        ExType lastExType();
        bool done();
        void advance();
        void operator ++();
    
    private:
        void process();
};

#endif