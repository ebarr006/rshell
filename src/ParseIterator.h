#ifndef PARSE_ITERATOR_H
#define PARSE_ITERATOR_H

#include <string.h>

enum ExType { COMMAND, CONNECTOR, OPEN_PAREN, CLOSE_PAREN, NONE };

class ParseIterator {
    
    protected:
        size_t pos;
        std::string &line;
        ExType last;
        bool treeStarted;
        int parenBalance;
    
    public:
        ParseIterator(size_t start, std::string &l) : pos(start), line(l), last(NONE), treeStarted(false), parenBalance(0) {}
        
        // Advances the iterator to the next Expression token (i.e. command, connector, parentheses...)
        void advance() {
            
            // Don't advance if at the end
            if(done()) {
                return;
            }
            
            std::string spec;
            size_t specLocation = findSpecialString(pos, spec);
            
            // Update last ExType
            last = currentExType();
            
            // Update treeStarted status
            if(currentExType() == COMMAND) {
                treeStarted = true;
            }
            if(currentExType() == OPEN_PAREN) {
                parenBalance++;
                treeStarted = false;
            }
            if(currentExType() == CLOSE_PAREN) {
                parenBalance--;
            }
            
            // Advance the iterator
            if(specLocation == pos) {
                pos += spec.size();
                specLocation = findSpecialString(pos, spec);
                if(isAllSpaces(substr2(line, pos, specLocation))) {
                    pos = specLocation;
                }
                return;
            }
            else {
                pos = specLocation;
                return;
            }
            
        }
        
        // Returns the string value of the current Expression token
        std::string getString() {
            std::string spec;
            size_t specLocation = findSpecialString(pos, spec);
            if(specLocation == pos) {
                return spec;
            }
            else {
                return substr2(line, pos, specLocation);
            }
        }
        
        // Returns the ExType of the current Expression token
        ExType currentExType() {
            std::string spec;
            size_t specLocation = findSpecialString(pos, spec);
            if(specLocation != pos) {
                return COMMAND;
            }
            else if(spec == "&&" || spec == "||" || spec == ";") {
                return CONNECTOR;
            }
            else if (spec == "(") {
                return OPEN_PAREN;
            }
            else if (spec == ")") {
                return CLOSE_PAREN;
            }
            else {
                return NONE;
            }
        }
        
        // Returns the ExType of the previous Expression token
        ExType lastExType() {
            return last;
        }
        
        // True when end of the line
        bool done() {
            return pos == std::string::npos || line.size() == 0 || pos >= line.size();
        }
        
        // Same as advance()
        void operator++() {
            advance();
        }
        
        // Checks for exceptions
        bool valid() {
            ExType curr = currentExType();
            if( (last == OPEN_PAREN  && curr == CONNECTOR       ) ||
                (last == CONNECTOR   && curr == CLOSE_PAREN     ) ||
                (last == OPEN_PAREN  && curr == CLOSE_PAREN     ) ||
                (last == CLOSE_PAREN && curr == OPEN_PAREN      ) ||
                (last == CONNECTOR   && curr == CONNECTOR       ) ||
                (last == COMMAND     && curr == COMMAND         ) ||
                (last == COMMAND     && curr == OPEN_PAREN      ) ||
                (last == CLOSE_PAREN && curr == COMMAND         ) ||
                (last == NONE        && curr == CLOSE_PAREN     ) ||
                (last == NONE        && curr == CONNECTOR       )
            )
            {
                if( (last == OPEN_PAREN  && curr == CONNECTOR       ) ){ std::cout << "open before connect" << std::endl; }
                if( (last == CONNECTOR   && curr == CLOSE_PAREN     ) ){ std::cout << "connect before close" << std::endl; }
                if( (last == OPEN_PAREN  && curr == CLOSE_PAREN     ) ){ std::cout << "open before close" << std::endl; }
                if( (last == CLOSE_PAREN && curr == OPEN_PAREN      ) ){ std::cout << "close before open" << std::endl; }
                if( (last == CONNECTOR   && curr == CONNECTOR       ) ){ std::cout << "connect before connect" << std::endl; }
                if( (last == COMMAND     && curr == COMMAND         ) ){ std::cout << "command before command" << std::endl; }
                if( (last == COMMAND     && curr == OPEN_PAREN      ) ){ std::cout << "command before open" << std::endl; }
                if( (last == CLOSE_PAREN && curr == COMMAND         ) ){ std::cout << "close before command" << std::endl; }
                if( (last == NONE        && curr == CLOSE_PAREN     ) ){ std::cout << "none before close" << std::endl; }
                if( (last == NONE        && curr == CONNECTOR       ) ){ std::cout << "none before connect" << std::endl; }
                
                return false;
                
            }
            return true;
        }
        
        // Checks if at the start of tree/substree
        bool atStart() {
            return !treeStarted;
        }
        
        
        bool isBalanced() {
            return (parenBalance == 0);
        }
    
    protected:
    
        // Converts an ExType to string for debugging
        std::string ExType_to_String(ExType e) {
            if(e == COMMAND) { return "COMMAND"; }
            if(e == CONNECTOR) { return "CONNECTOR"; }
            if(e == OPEN_PAREN) { return "OPEN_PAREN"; }
            if(e == CLOSE_PAREN) { return "CLOSE_PAREN"; }
            if(e == NONE) { return "NONE"; }
            return "BLEH";
        }
        
        
        // Java-like substring method
        std::string substr2(std::string &s, size_t start, size_t end) {
            if(end == std::string::npos) 
            {
                return s.substr(start);
            }
            else 
            {
                return s.substr(start, end - start);
            }
        }
        
        // Returns pos of earliest special string and ref-returns the special string
        size_t findSpecialString(size_t startingPos, std::string& specialString) {
            
            
            std::vector<std::string> specialStrings = { "&&", "||", ";", "(", ")"};
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
        
        // True if s is only spaces
        bool isAllSpaces(std::string s) {
            for(char c: s) {
                if(c != ' ') {
                    return false;
                }
            }
            return true;
        }
};


#endif