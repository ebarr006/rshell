#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "Expression.h"
#include "And.h"
#include "Or.h"
#include "Semi.h"
#include "Command.h"
#include "ParseIterator.h"

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
        bool createTree(ParseIterator &i, Expression*& subroot, bool isSubTree);
};

#endif