#include "Parser.h"

// Initializes the object with empty string
Parser::Parser() {
    this->setString("");
    root = nullptr;
}


// Initializes the object and omits leading spaces and anything after the # symbol.
Parser::Parser(std::string s) {
    this->setString(s);
    root = nullptr;
}


// Updates the line of commands. Automatically omits leading spaces and anything after the # symbol.
void Parser::setString(std::string s) {
    
    if(s.size() == 0)
    {
        line = "";
        return;
    }
    
    // Removes comments
    if(s.find('#') != std::string::npos) {
        s = s.substr(0, s.find('#'));
    }
    
    // Removes leading spaces
    size_t j = 0;
    while(j < s.size() && s.at(j)==' ') { j++; }
    s = s.substr(j);

    line = s;
}


// Converts the line of commands into an Expression tree.
bool Parser::parse(Expression*& express) {
    
    if (line.size() == 0) {
        delete root;
        root = express = nullptr;
        return false;
    }
    
    // Check if line of commands starts with a connector. If so, throw error.
    if (line.substr(0,1) == ";" || line.substr(0,2) == "&&" || line.substr(0,2) == "||") {
        std::cout << "Syntax error: Cannot start with connector" << std::endl;
        return false;
    }
    
    delete root;                    // Delete old tree
    
    enum Seen { COMMAND, CONNECTOR };
    
    size_t currentLoc = 0;             // Keeps track of current parse location
    Seen lastSeen = COMMAND;        // Keeps track of last parsed object
    std::string specialStr = "";    // Stores last special string encountered
    size_t specialLoc = this->findSpecialString(currentLoc, specialStr);   // Stores location of next special string

    // Precondition: First substring is a command. (We have not implemented parentheses yet.)
    // 1. First branch is to the left
    root = new Command(line.substr(currentLoc, specialLoc));
    currentLoc = specialLoc;
    
    // 2. Loop
    while(currentLoc != std::string::npos && currentLoc < line.size() - 1) {
        
        specialLoc = this->findSpecialString(currentLoc, specialStr);
        // Special string
        if(currentLoc == specialLoc) {
            
            if(specialStr == "&&" || specialStr == "||" || specialStr == ";") {
                
                // Check if last parsed object was connector. If so, throw error.
                if (lastSeen == CONNECTOR) {
                    delete root;
                    root = express = nullptr;
                    std::cout << "Syntax error: Double connector" << std::endl;
                    return false;
                }
                

                // Otherwise, create a connector object in the tree.
                if     (specialStr == "&&") {  root = new And(root);  }
                else if(specialStr == "||") {  root = new Or(root);   }
                else if(specialStr == ";")  {  root = new Semi(root); }
                
                currentLoc += specialStr.size();
                
                lastSeen = CONNECTOR;
                continue;
            }
            else {
                delete root;
                return false;
            }

        }
        // Spaces
        else if(isAllSpaces(substr2(line, currentLoc, specialLoc))) {
            currentLoc = specialLoc;
            continue;
        }
        // Command
        else {
            
            // Check if last parsed object was command (THIS SHOULD NEVER HAPPEN). If so, throw error.
            if (lastSeen == COMMAND) {
                delete root;
                root = express = nullptr;
                std::cout << "Internal error: Double command" << std::endl;
                return false;
            }
            
            
            Expression* newCommand = new Command(substr2(line, currentLoc, specialLoc));
            if(!(root->addChild(newCommand))) {
                delete newCommand;
                delete root;
                root = express = nullptr;
                std::cout << "Internal error: Connector already has two children" << std::endl;
                return false;
            }
            
            currentLoc = specialLoc;
            
            lastSeen = COMMAND;
            continue;
        }
    }
    
    //Check if line of commands ends with a binary connector
    if(lastSeen == CONNECTOR) {
        if(specialStr == "&&" || specialStr == "||") {
            delete root;
            root = express = nullptr;
            std::cout << "Syntax error: Cannot end with &&, || connectors" << std::endl;
            return false;
        }
    }
    
    
    
    
    express = root;
    
    return true;
    
}


// Returns the index of the next special string. The second parameter is used to return the special string found.
size_t Parser::findSpecialString(size_t startingPos, std::string& specialString) {
    
    
    std::vector<std::string> specialStrings = { "&&", "||", ";"};
    size_t m = std::string::npos;
    std::string encounter;
    
    for(std::string s: specialStrings) {
        size_t f = line.find(s, startingPos);
        if(f != std::string::npos && ((f < m) || (m == std::string::npos))) {
            m = f;
            encounter = s;
        }
    }
    
    if(m != std::string::npos) {
        specialString = encounter;
    }
    
    return m;
    
}


// Returns true if the string is composed of only spaces. Returns false if at least one character is not a space.
bool Parser::isAllSpaces(std::string s) {
    for(char c: s) {
        if(c != ' ') {
            return false;
        }
    }
    return true;
}


// Java-like substring function (for convenience)
std::string Parser::substr2(std::string &s, size_t start, size_t end) {
    if(end == std::string::npos) 
    {
        return s.substr(start);
    }
    else 
    {
        return s.substr(start, end - start);
    }
}

