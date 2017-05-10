#include "Semi.h"

bool Semi::execute() {
    left->execute();
    return right->execute();
}

Semi::~Semi() {
    if(left != nullptr) { delete left; }
    if(right != nullptr){ delete right; }
}