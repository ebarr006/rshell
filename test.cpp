#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>

int main(int argc, char **argv) {
    if(argc != 2) {
        return 1;
    }
 
    struct stat fileStat;
    
    // std::cout << "HERE" << std::endl;
    
    if(stat(argv[1],&fileStat) < 0) { 
        std::cout << "HERE" << std::endl;
        return 1;
    }
    
    std::cout << "Information for " << argv[1] << std::endl;
    std::cout << "---------------------------\n";
    
    std::cout << "testing S_ISDIR\n";
    
    if (S_ISREG(fileStat.st_mode)) {
        std::cout << "this works" << std::endl;
    }
    return 0;
}