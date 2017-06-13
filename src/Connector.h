#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "Expression.h"
#include <iostream>

class Connector : public Expression {
    protected:
        Expression* left;
        Expression* right;
        
    public:
        Connector(Expression* l, Expression* r) : left(l), right(r) {}
        virtual ~Connector() {}
        virtual bool execute() = 0;
        virtual bool addChild(Expression* e);
        virtual Expression*& getRight();
        
};

#endif