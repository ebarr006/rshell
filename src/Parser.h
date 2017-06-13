#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "Expression.h"
#include "And.h"
#include "Or.h"
#include "Semi.h"
#include "Sentence.h"
#include "Phrase.h"
#include "ParseIterator.h"

class Parser {
    protected:
        std::string line;
        Expression* root;
        size_t paren_count = 0;
        
        //USED ONLY BY parse() and createTree()
        //bool atStartOfTree = true;
        
    public:
        
        // c'tors
        Parser();
        Parser(std::string s);
        
        // methods
        void setString(std::string s);
        bool parse(Expression*& express);
        bool isBalanced();
    
    private:
        // helper functions
        bool valid(ExType, ExType);
        bool createTree(ParseIterator &i, Expression*& subroot, bool isSubTree);
        std::string ExType_to_String(ExType);
        char** createArgArray_FromString(std::string s, int& argSize);
        
};

#endif