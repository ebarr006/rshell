#include "Sentence.h"

// Properly deallocates char array upon destruction
Sentence::~Sentence() {
    for(Phrase* p: Brian) {
        delete[] p->argArray;
        delete p;
    }
}


// Executes bash/built-in command
bool Sentence::execute() {
    
    int status;
    int prev_in = 0;
    int myPipe[2];
    
    
    for(unsigned i = 0; i < Brian.size(); ++i) {
        pipe(myPipe);
        pid_t pid = fork();
        
        // Check if fork successful
        if(pid < 0) {
            perror("fork failed");
            exit(2);
        }
        
        // Child process
        if(pid == 0) {
            
            // Overwrite STDIN with prev_in
            dup2(prev_in, 0);
            
            int in_fd = -1;
            int out_fd = -1;
            
            
            // Overwrite STDIN with inFile
            if(Brian.at(i)->inFile != "") {
                
                in_fd = open(Brian.at(i)->inFile.c_str(), O_RDONLY);
                if(in_fd < 0) {
                    perror("couldn't open file");
                    exit(3);
                }
                dup2(in_fd, 0);
            }
            
            // Overwrite STDOUT with outFile
            if (Brian.at(i)->outFile != "") {
                if (Brian.at(i)->appendModeOn) {
                    out_fd = open(Brian.at(i)->outFile.c_str(), O_WRONLY | O_APPEND | O_CREAT, S_IWUSR | S_IRGRP);
                }
                else {
                    out_fd = open(Brian.at(i)->outFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRGRP);
                }
                if(out_fd < 0) {
                    perror("couldn't open/create file");
                    exit(4);
                }
                dup2(out_fd, 1);
            }
            else {
                // Overwrite STDOUT with write-end of myPipe
                if(i != Brian.size() - 1) {
                    dup2(myPipe[1], 1);
                }
                
            }    
            // Close read-end of myPipe
            close(myPipe[0]);
            
            // Execute command
            
            int exitCode;
            
            if(strcmp(Brian.at(i)->argArray[0], "test") == 0) {
                exitCode = testRunner(Brian.at(i)->argArray, Brian.at(i)->argSize);
            }
            else if (strcmp(Brian.at(i)->argArray[0], "EXIT") == 0) {
                exitCode = exitRunner(Brian.at(i)->argArray, Brian.at(i)->argSize);
                exit(100);
            }
            else {
                exitCode = bashRunner(Brian.at(i)->argArray, Brian.at(i)->argSize);
            }
            
            if(in_fd != -1)  { close(in_fd); }
            if(out_fd != -1) { close (out_fd); }
            
            if(exitCode < 0) {
                perror("execvp failed");
                exit(1);
            }
            else if(exitCode == 120){   // FALSE exitcode
                exit(120);
            }
            else if(exitCode == 121) {  // TRUE exitcode
                exit(121);
            }
            
            exit(0);
        }
        
        // Parent process
        close(myPipe[1]);
        waitpid(-1, &status, 0);
        if(WIFEXITED(status)){
            if(WEXITSTATUS(status) == 100) {
                exit(0);
            }
            else if(WEXITSTATUS(status) == 120) {
                return false;
            }
            else if(WEXITSTATUS(status) == 121) {
                return true;
            }
            else if(WEXITSTATUS(status) != 0) {
                return false;
            }
        }
        prev_in = myPipe[0];
    }
    return true;
}


// Implemented because interface requires it. BUT should never be called.
// If called, return an error signal of FALSE, since a Sentence cannot have children.
bool Sentence::addChild(Expression* e) {
    return false;
}

Expression*& Sentence::getRight() {
    return dummy;
}


void Sentence::addPhrase(Phrase* p) {
    Brian.push_back(p);
}

bool Sentence::empty() {
    return Brian.empty();
}

int Sentence::testRunner(char** argArray, unsigned argSize) {
    if (argSize < 2) {
        std::cout << "(False)" << std::endl;
        return 120;
    }
    
    struct stat fileStat;
    
    if(strcmp(argArray[1], "-e") == 0 || strcmp(argArray[1], "-f") == 0 || strcmp(argArray[1], "-d") == 0) {
    
        bool exists = (stat(argArray[2],&fileStat) >= 0) ;
        
        if(exists) {
            if (strcmp(argArray[1], "-e") == 0 ||
                (strcmp(argArray[1], "-f") == 0 && S_ISREG(fileStat.st_mode)) ||
                (strcmp(argArray[1], "-d") == 0 && S_ISDIR(fileStat.st_mode)))
            {
                std::cout << "(True)" << std::endl;
                return 121;
            }
        }
        else {
            std::cout << "(False)" << std::endl;
            return 120;
        }

    }
    
    if(stat(argArray[1],&fileStat) < 0) { 
        std::cout << "(False)" << std::endl;
        return 120;
    }
    else {
        std::cout << "(True)" << std::endl;
        return 121;
    }
    
    return 120;
}

int Sentence::exitRunner(char** argArray, unsigned argSize) {
    std::cout << "Exited Rshell." << std::endl;
    return 1;
}

int Sentence::bashRunner(char** argArray, unsigned argSize) {
    return execvp(argArray[0], argArray);
}


// The stuff that was in CommandRunner:
/*
class CommandRunner {
    protected:
        char** argArray;
        unsigned argSize;
        
    public:
        CommandRunner(){};
        CommandRunner(char** ca, unsigned as) : argArray(ca), argSize(as) {};
        virtual bool execute() = 0;
};



class Exit_Runner : public CommandRunner {
    public:
        Exit_Runner(char** ca, unsigned as) : CommandRunner(ca, as) {}
        virtual bool execute() {
            std::cout << "Exited Rshell." << std::endl;
            exit(0);
        }
};



class Bash_Runner : public CommandRunner {
    public:
        Bash_Runner(char** ca, unsigned as) : CommandRunner(ca, as) {}
        virtual bool execute() {
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
};



class Test_Runner: public CommandRunner {
    public:
        Test_Runner(char** ca, unsigned as) : CommandRunner(ca, as) {}
        virtual bool execute() { 
            
            if (argSize < 2) {
                std::cout << "(False)" << std::endl;
                return false;
            }
            
            struct stat fileStat;
            
            if(strcmp(argArray[1], "-e") == 0 || strcmp(argArray[1], "-f") == 0 || strcmp(argArray[1], "-d") == 0) {
            
                bool exists = (stat(argArray[2],&fileStat) >= 0) ;
                
                if(exists) {
                    if (strcmp(argArray[1], "-e") == 0 ||
                        (strcmp(argArray[1], "-f") == 0 && S_ISREG(fileStat.st_mode)) ||
                        (strcmp(argArray[1], "-d") == 0 && S_ISDIR(fileStat.st_mode)))
                    {
                        std::cout << "(True)" << std::endl;
                        return true;
                    }
                }
                else {
                    std::cout << "(False)" << std::endl;
                    return false;
                }

            }
            
            if(stat(argArray[1],&fileStat) < 0) { 
                std::cout << "(False)" << std::endl;
                return false;
            }
            else {
                std::cout << "(True)" << std::endl;
                return true;
            }
            
            return false; 
        }
    
};
*/