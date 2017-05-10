#include "And.h"

bool And::execute() {
    return left->execute() && right->execute();
}

And::~And() {
    if(left != nullptr) { delete left; }
    if(right != nullptr){ delete right; }
}