# Rshell
_By [Emilio Barreiro](https://github.com/ebarr006 "Emilio's Github") and [Kevin Huang](https://github.com/khuan028 "Kevin's Github")_

__Rshell__ is a C++ command shell that can execute bash commands and custom built-in commands.
It was created as a student project for CS100.



## Installation

You must have the _C++ Boost Library_ installed before continuing.

Clone and setup the _Rshell_ using the following commands:
```console
git clone  https://github.com/ebarr006/rshell.git
cd rshell
git checkout hw3
make
```


## Starting RShell

Start the _Rshell_ using the following command:
```console
bin/rshell
```



## Usage

_Rshell_ functions like a bash command shell. It currently supports:

Ordinary bash commands
```console
mkdir foodPics
echo hello world
cat events
```
Compound commands
```console
vim taxes; ls -a && cd.. || rm credits

```
Compound commands w/ parentheses
``` console
cat nonexistant.txt && (echo hi; ls -l) || ((echo world))
```
Tests
``` console
test -d DonutShop/finances/revenue
[DonutShop/finances/revenue/Q1.xls]
```
However, _Rshell_ currently __does not support piping__. For example, the following line would fail:
```console
ls -a > myTextFile
```



## Built-in Commands
The _Rshell_ has its own built-in `EXIT` command for exiting the shell:
```console
EXIT
```


## Important Information
__NOTE:__ There are two Makefiles in our rshell repository. However, following the assignment specifications, there is only one contained within the Root directory. The other is inside /src/ and solely involves compiling the source files. The main Makefile, again within root, handles properly organizing the executable inside /bin. Our repository is set up so that the program will still build and run according to the specifications.



## Known Bugs
1. Piping is not supported
2. Sometimes, `EXIT` won't work

## License
GNU General Public License v3.0

Copyright (C) 2017  Emilio Barreiro, Kevin Huang
