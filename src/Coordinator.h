#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <iostream>
#include "Parser.h"

class Coordinator {
    private:
        Parser myParser;
        Expression *tree;
        
    public:
        Coordinator();
        void start();
    
    private:
        bool run();
};

#endif