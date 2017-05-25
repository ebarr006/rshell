#ifndef COMMAND_H
#define COMMAND_H

#include "Expression.h"
#include "CommandRunner.h"
#include <cstdlib>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <boost/tokenizer.hpp>

typedef boost::char_separator<char> Separator;
typedef boost::tokenizer<Separator> Tokenizer;

class Command : public Expression {
    
    private:
        char** argArray;
        unsigned argSize;
        std::vector<std::string> argVector;
        
        // Because getRight needs a variable, we create this dummy variable (I know, it doesn't make sense.)
        Expression* dummy;
    
    public:
        Command(std::string);
        ~Command();
        virtual bool execute();
        virtual bool addChild(Expression* e);
        virtual Expression*& getRight();
    
};

#endif