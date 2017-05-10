#ifndef OR_H
#define OR_H

#include "Connector.h"
#include <iostream>

class Or : public Connector {
    public:
        Or() : Connector(nullptr, nullptr){}
        Or(Expression *a) : Connector(a, nullptr){}
        Or(Expression *a, Expression *b) : Connector(a, b) {}
        ~Or();
        bool execute();
};

#endif