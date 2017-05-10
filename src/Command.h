#ifndef COMMAND_H
#define COMMAND_H

#include "Expression.h"
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
        std::vector<std::string> argVector;
    
    public:
        Command(std::string);
        ~Command();
        virtual bool execute();
        virtual bool addChild(Expression* e);
    
};

#endif