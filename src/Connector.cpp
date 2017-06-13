#include "Connector.h"

// Adds a child if possible.
// Returns true if there is space to add children. Returns false if two children already exist.
bool Connector::addChild(Expression *e) {
    if(this->left == nullptr) {
        this->left = e;
        return true;
    }
    else if(this->right == nullptr) {
        this->right = e;
        return true;
    }
    else {
        return false;
    }
}


Expression*& Connector::getRight() {
    return right;
}

