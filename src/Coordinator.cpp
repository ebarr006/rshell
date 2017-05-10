#include "Coordinator.h"

Coordinator::Coordinator() : tree(nullptr) {}

// The loop in which the user inputs commands and the Rshell executes the commands
// The loop ends when EOF is detected.
void Coordinator::start() {
    std::string input;
    std::cout << "$ ";
    while(std::getline(std::cin, input)) {
        this->myParser.setString(input);    // sets up tree creation
        this->myParser.parse(tree);         // creates executable tree
        this->run();                        // executes the tree
        std::cout << "$ ";
    }
}

// Executes the line of commands
bool Coordinator::run() {
    if(tree == nullptr) {
        return false;
    }
    else {
        tree->execute();
        return true;
    }
}