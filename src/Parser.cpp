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

    if(success && isBalanced()) {
        express = root;
        return true;
    }
    else {
        if(!isBalanced()) {
            std::cout << "Parse error: Unbalanced parentheses! " << std::endl;
        }
        std::cout << "Error in parsing" << std::endl;
        delete root;
        root = nullptr;
        return false;
    }
    
    return false;
    
}


// Checks if # open paren == # close paren
bool Parser::isBalanced() {
    return (paren_count == 0);
}


// Constructs the Expression tree
bool Parser::createTree(ParseIterator &i, Expression*& subroot, bool isSubTree) {
    
    bool atStartOfTree = true;
    
    Sentence *sentence = nullptr;
    Phrase *phrase = nullptr;
    ExType cur;
    ExType pre;
    std::string stri;

    for(; !i.done(); ++i) {
        
        cur = i.currentExType();
        pre = i.lastExType();
        stri = i.getString();
        
        
        
        
        // Check exceptions regarding adjacent ExTypes
        if (!valid(pre, cur)) {
            std::cout << "Parse: Syntax error! " << stri << std::endl;
            delete sentence;
            delete phrase;
            sentence = nullptr;
            phrase = nullptr;
            return false;
        }
        
        if(cur == COMMAND ||  cur == PIPE || cur == FILENAME || cur == RIGHT_ARROW || cur == LEFT_ARROW || cur == DOUBLE_RIGHT_ARROW) 
        {
                // Instantiates objects if nonexistant
                if(sentence == nullptr) {
                    sentence = new Sentence();
                }
                
                if(phrase == nullptr) {
                    phrase = new Phrase();
                }
                
                
                // Does stuff
                if(cur == COMMAND) {
                    if(phrase->argArray != nullptr) {       //Checks if argArray already contains a command
                        delete sentence;
                        delete phrase;
                        sentence = nullptr;
                        phrase = nullptr;
                        return false;
                    }
                    else {
                        phrase->argArray = createArgArray_FromString(stri, phrase->argSize);

                    }
                }
                
                else if(cur == PIPE) {
                    if(phrase->argArray != nullptr) {       //Checks that argArray contains a command
                        sentence->addPhrase(phrase);
                    }
                    phrase = nullptr;
                }
                
                else if(cur == FILENAME) {
                    if(pre == RIGHT_ARROW) { 
                        phrase->outFile = stri;
                        phrase->appendModeOn = false;
                    }
                    else if(pre == DOUBLE_RIGHT_ARROW) {
                        phrase->outFile = stri;
                        phrase->appendModeOn = true;
                    }
                    else if(pre == LEFT_ARROW) {
                        phrase->inFile = stri;
                    }
                    
                }
            
        }
        else 
        {
            // Add the sentence node to the tree (if there is a sentence at all)
            if(sentence != nullptr) {
                
                if(phrase != nullptr) {
                    if(phrase->argArray == nullptr) {
                        std::cout << "Parse: Syntax error! No command!" << std::endl;
                        return false;
                    }
                    sentence->addPhrase(phrase);
                    phrase = nullptr;
                }
                if(atStartOfTree) {
                    subroot = sentence;
                }
                else {
                    subroot->addChild(sentence);
                }
                atStartOfTree = false;
                sentence = nullptr;
                
            }
            
            if(cur == CONNECTOR) {
                if(stri == "&&") { subroot = new And(subroot);  }
                if(stri == "||") { subroot = new Or(subroot);   }
                if(stri == ";")  { subroot = new Semi(subroot); }
            }
            
            else if(cur == OPEN_PAREN) {
                Expression*& nextRoot = (atStartOfTree) ? subroot : subroot->getRight();
                ++i;
                
                if(!createTree(i, nextRoot, true)) {
                    if(!isSubTree) {
                        std::cout << "Parse error: Missing closing parentheses! " << stri << std::endl;
                        delete sentence;
                        delete phrase;
                        sentence = nullptr;
                        phrase = nullptr;
                    }
                    return false;
                }
                
                atStartOfTree = false;  //Now that we have a subtree, we are not at the start of a tree
            }
            
            else if (cur == CLOSE_PAREN && isSubTree) {
                if(sentence != nullptr) {
                    
                    if(phrase != nullptr) {
                        if(phrase->argArray == nullptr) {
                            std::cout << "Parse: Syntax error! No command!" << std::endl;
                            delete sentence;
                            delete phrase;
                            sentence = nullptr;
                            phrase = nullptr;
                            return false;
                        }
                        sentence->addPhrase(phrase);
                        phrase = nullptr;
                    }
                    if(atStartOfTree) {
                        subroot = sentence;
                    }
                    else {
                        subroot->addChild(sentence);
                    }
                    atStartOfTree = false;
                    sentence = nullptr;
                    
                }
                return true;
            }
        }
    }
    // back to the top of the for loop
    
    
    
    
    
    
    
    
    
    
    
    
    
    // We have reached the end...
    if(isSubTree) { 
        delete sentence;
        delete phrase;
        sentence = nullptr;
        phrase = nullptr;
        return false;  // ...but our subtree is missing a closing parentheses. Return false.
    }  
    else {  
        // Add the sentence node to the tree (if there is a sentence at all)

        if(sentence != nullptr) {
            
            if(phrase != nullptr) {
                if(phrase->argArray == nullptr) {
                    std::cout << "Parse: Syntax error! No command!" << std::endl;
                    delete sentence;
                    delete phrase;
                    sentence = nullptr;
                    phrase = nullptr;
                    return false;
                }
                sentence->addPhrase(phrase);
                phrase = nullptr;
                
            }
            if(atStartOfTree) {
                subroot = sentence;
            }
            else {
                subroot->addChild(sentence);
            }
            atStartOfTree = false;
            sentence = nullptr;
            
        }
        
        if(cur == CONNECTOR){
            std::cout << "Parse: Syntax error! Connector at end of line!" << std::endl;
            delete sentence;
            delete phrase;
            sentence = nullptr;
            phrase = nullptr;
            return false;
        }
        
        
        
        
        return true;   // ...and our base tree don't need a closing parentheses. Return true.
    }  
    
     
}


// Checks if adjacent exTypes are compatible
bool Parser::valid(ExType last, ExType current) {
            
    if     ( (last == OPEN_PAREN         && current == CONNECTOR    ) ){ std::cout << "open before connect" << std::endl; }
    else if( (last == OPEN_PAREN         && current == CLOSE_PAREN  ) ){ std::cout << "open before close" << std::endl; }
    else if( (last == CLOSE_PAREN        && current == OPEN_PAREN   ) ){ std::cout << "close before open" << std::endl; }
    else if( (last == CLOSE_PAREN        && current == COMMAND      ) ){ std::cout << "close before command" << std::endl; }
    else if( (last == CONNECTOR          && current == CLOSE_PAREN  ) ){ std::cout << "connect before close" << std::endl; }
    else if( (last == CONNECTOR          && current == CONNECTOR    ) ){ std::cout << "connect before connect" << std::endl; }
    else if( (last == COMMAND            && current == COMMAND      ) ){ std::cout << "command before command" << std::endl; }
    else if( (last == COMMAND            && current == OPEN_PAREN   ) ){ std::cout << "command before open" << std::endl; }
    else if( (last == NONE               && current == CLOSE_PAREN  ) ){ std::cout << "none before close" << std::endl; }
    else if( (last == NONE               && current == CONNECTOR    ) ){ std::cout << "none before connect" << std::endl; }
    else if( (last == PIPE               && current == PIPE         ) ){ std::cout << "pipe before pipe" << std::endl; }
    else if( (last == PIPE               && current == CONNECTOR    ) ){ std::cout << "pipe before connect" << std::endl; }
    else if( (last == PIPE               && current == OPEN_PAREN   ) ){ std::cout << "pipe before open" << std::endl; }
    else if( (last == PIPE               && current == CLOSE_PAREN  ) ){ std::cout << "pipe before close" << std::endl; }
    else if( ((last == RIGHT_ARROW ||  last == LEFT_ARROW || last == DOUBLE_RIGHT_ARROW) && current != FILENAME) ){ std::cout << "redir not followed by filename" << std::endl; }
    else { return true; }
    return false;
}


// Converts ExType to string (for debugging)
std::string Parser::ExType_to_String(ExType e) {
    if(e == COMMAND) { return "COMMAND"; }
    if(e == CONNECTOR) { return "CONNECTOR"; }
    if(e == OPEN_PAREN) { return "OPEN_PAREN"; }
    if(e == CLOSE_PAREN) { return "CLOSE_PAREN"; }
    if(e == RIGHT_ARROW) { return "RIGHT_ARROW"; }
    if(e == LEFT_ARROW) { return "LEFT_ARROW"; }
    if(e == DOUBLE_RIGHT_ARROW) { return "DOUBLE_RIGHT_ARROW"; }
    if(e == PIPE) { return "PIPE"; }
    if(e == FILENAME) { return "FILENAME"; }
    if(e == NONE) { return "NONE"; }
    return "BLEH";
}


// Dynamically allocates and creates an argument array from a string
char** Parser::createArgArray_FromString(std::string cmd, int &argSize) {
    
    typedef boost::char_separator<char> Separator;
    typedef boost::tokenizer<Separator> Tokenizer;
    
    Separator sep(" []");
    Tokenizer tok(cmd, sep);
    std::vector<std::string> argVector;
    char** argArray = nullptr;
    
    if(cmd.find('[') != std::string::npos && cmd.find(']') != std::string::npos) {
        argVector.push_back("test");
    }
    
    
    for(Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        argVector.push_back(*it);
    }

    argArray = new char*[argVector.size() + 1];
    argArray[argVector.size()] = NULL;
    
    for(unsigned i = 0; i < argVector.size(); ++i) {
        /*argArray[i] = (char*) argVector.at(i).c_str();*/
        char* c = new char[argVector.at(i).size()];
        strcpy(c, argVector.at(i).c_str());
        argArray[i] = c;
    }
    
    argSize = argVector.size();
    return argArray;
}