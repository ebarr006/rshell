#include "Or.h"

bool Or::execute() {
    return left->execute() || right->execute();
}

Or::~Or() {
    if(left != nullptr) { delete left; }
    if(right != nullptr){ delete right; }
}