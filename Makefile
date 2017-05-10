CC=g++
CC_FLAGS=-Wall -ansi -std=c++11
EXEC=rshell
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

all: rshell

rshell:
	mkdir -p ./bin
	cd src && make
	$(CC) src/*.o -o ./bin/$(EXEC)
clean:
	cd src && make clean
	rm -f ./bin/$(EXEC) $(OBJECTS)
