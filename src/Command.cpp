#include "Command.h"

// Converts string into a char array that execvp() can work with
Command::Command(std::string cmd) {
    Separator sep(" []");
    Tokenizer tok(cmd, sep);
    this->argVector.clear();
    
    if(cmd.find('[') != std::string::npos && cmd.find(']') != std::string::npos) {
        this->argVector.push_back("test");
    }
    
    
    for(Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        this->argVector.push_back(*it);
    }

    this->argArray = new char*[this->argVector.size() + 1];
    this->argArray[this->argVector.size()] = NULL;
    
    for(unsigned i = 0; i < this->argVector.size(); ++i) {
        this->argArray[i] = (char*) argVector.at(i).c_str();
    }
    
    argSize = this->argVector.size();
    
}


// Properly deallocates char array upon destruction
Command::~Command() {
    delete[] this->argArray;
}


// Executes bash/built-in command
bool Command::execute() {
    
    
    CommandRunner* runner = nullptr;
    
    // exit command
    if(strcmp(argArray[0], "EXIT") == 0) {
        runner = new Exit_Runner(argArray, argSize);
    } 
    // test command
    else if(strcmp(argArray[0], "test") == 0) {
        runner = new Test_Runner(argArray, argSize);
    }
    // bash commands
    else {
        runner = new Bash_Runner(argArray, argSize);
    }
    
    return runner->execute();

}


// Implemented because interface requires it. BUT should never be called.
// If called, return an error signal of FALSE, since a Command cannot have children.
bool Command::addChild(Expression* e) {
    return false;
}

Expression*& Command::getRight() {
    return dummy;
}

