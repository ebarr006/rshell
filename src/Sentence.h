/*
    Description: A SENTENCE consists of multiple PHRASES connected by pipes.
*/

#ifndef SENTENCE_H
#define SENTENCE_H

#include "Expression.h"
#include <cstdlib>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <boost/tokenizer.hpp>
#include "Phrase.h"

class Sentence : public Expression {
    
    private:
        std::vector<Phrase*> Brian;
        // Because getRight needs a variable, we create this dummy variable (I know, it doesn't make sense.)
        Expression* dummy;

    
    public:
        Sentence(){};
        ~Sentence();
        virtual bool execute();
        virtual bool addChild(Expression* e);
        virtual Expression*& getRight();
        void addPhrase(Phrase* p);
        bool empty();
        
        int testRunner(char** argArray, unsigned argSize);
        int exitRunner(char** argArray, unsigned argSize);
        int bashRunner(char** argArray, unsigned argSize);
        

};

#endif