#ifndef COMMAND_RUNNER_H
#define COMMAND_RUNNER_H

#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <iostream>

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

#endif