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
    
    delete root;
    root = express = nullptr;
    
    ParseIterator it(0, line);
    
    bool success = createTree(it, root, false);
    if(success && it.isBalanced()) {
        express = root;
        return true;
    }
    else {
        if(!it.isBalanced()) {
            std::cout << "Parse error: Unbalanced parentheses! " << std::endl;
        }
        delete root;
        root = nullptr;
        return false;
    }
    
    return false;
    
}

bool Parser::createTree(ParseIterator &i, Expression*& subroot, bool isSubTree) {
    for(; !i.done(); ++i) {
        
        // Check exceptions
        if (!i.valid()) {
            std::cout << "Parse: Syntax error! " << i.getString() << std::endl;
            return false;
        }
        
        // If no exceptions, then build tree
        if(i.currentExType() == COMMAND) {
            if(i.atStart()) {
                subroot = new Command(i.getString());
            }
            else {
                subroot->addChild( new Command(i.getString()) );
            }

        }
        
        else if(i.currentExType() == CONNECTOR) {
            if(i.getString() == "&&") { subroot = new And(subroot);  }
            if(i.getString() == "||") { subroot = new Or(subroot);   }
            if(i.getString() == ";")  { subroot = new Semi(subroot); }
        }
        
        else if(i.currentExType() == OPEN_PAREN) {
            Expression*& nextRoot = (i.atStart()) ? subroot : subroot->getRight();
            ++i;
            if(!createTree(i, nextRoot, true)) {
                if(!isSubTree) {
                    std::cout << "Parse error: Missing closing parentheses! " << i.getString() << std::endl;
                }
                return false;
            }
        }
        
        else if (i.currentExType() == CLOSE_PAREN && isSubTree) {
            return true;
        }
        
    }
    
    // We have reached the end...
    if(isSubTree) {  return false; }  // ...but our subtree is missing a closing parentheses. Return false.
    else          {  return true;  }   // ...and our base tree don't need a closing parentheses. Return true.
    
     
}

