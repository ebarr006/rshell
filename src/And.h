#ifndef AND_H
#define AND_H

#include "Connector.h"

class And : public Connector {
    public:
        And() : Connector(nullptr, nullptr){}
        And(Expression *a) : Connector(a, nullptr){}
        And(Expression *a, Expression *b) : Connector(a, b) {}
        ~And();
        bool execute();
};

#endif