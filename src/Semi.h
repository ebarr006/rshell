#ifndef SEMI_H
#define SEMI_H

#include "Connector.h"

class Semi : public Connector {
    public:
        Semi() : Connector(nullptr, nullptr){}
        Semi(Expression *a) : Connector(a, nullptr){}
        Semi(Expression *a, Expression *b) : Connector(a, b) {}
        ~Semi();
        bool execute();
};

#endif