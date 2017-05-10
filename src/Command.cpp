#include "Command.h"

// Converts string into a char array that execvp() can work with
Command::Command(std::string cmd) {
    Separator sep(" ");
    Tokenizer tok(cmd, sep);
    this->argVector.clear();
    
    for(Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        this->argVector.push_back(*it);
    }

    this->argArray = new char*[this->argVector.size() + 1];
    this->argArray[this->argVector.size()] = NULL;
    
    for(unsigned i = 0; i < this->argVector.size(); ++i) {
        this->argArray[i] = (char*) argVector.at(i).c_str();
    }
    
}


// Properly deallocates char array upon destruction
Command::~Command() {
    delete[] this->argArray;
}


// Executes bash/built-in command
bool Command::execute() {
    
    // built-in commands
    if(strcmp(argArray[0], "EXIT") == 0) {
        std::cout << "Exited Rshell." << std::endl;
        exit(0);
    }
    
    
    
    
    
    
    // bash/linux commands
    int status = 0;
    pid_t pid = fork();
    
    if(pid == 0) {
        execvp(argArray[0], argArray);
        perror("Command error");
        return false;
    }
    else if(pid > 0) {
        waitpid(-1, &status, 0);
        return true;
    }
    else {
        std::cout << "ERROR: Fork failed unexpectedly. Exiting immediately." << std::endl;
        exit(1);
    }
    
    
    
    return false;
}


// Implemented because interface requires it. BUT should never be called.
// If called, return an error signal of FALSE, since a Command cannot have children.
bool Command::addChild(Expression* e) {
    return false;
}

