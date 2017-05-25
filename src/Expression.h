#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>

// Expression is an abstract base class for commands and connectors.
// This makes it possible to create an Expression tree.
class Expression {
    public:
        virtual ~Expression() {};
        virtual bool execute() = 0;
        virtual bool addChild(Expression* e) = 0;
        virtual Expression*& getRight() = 0;
};

#endif