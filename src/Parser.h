#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "Expression.h"
#include "And.h"
#include "Or.h"
#include "Semi.h"
#include "Command.h"

class Parser {
    protected:
        std::string line;
        Expression* root;
        
    public:
        // c'tors
        Parser();
        Parser(std::string s);
        
        // methods
        void setString(std::string s);
        bool parse(Expression*& express);
    
    private:
        // helper functions
        size_t findSpecialString(size_t startingPos, std::string& specialString);
        bool isAllSpaces(std::string s);
        std::string substr2(std::string &s, size_t start, size_t end);
};

#endif