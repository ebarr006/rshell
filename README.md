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
git checkout hw2
make
```
## Start up

Start the _Rshell_ using the following command:
```console
bin/rshell
```

## Usage

_Rshell_ functions like any bash command shell. You can execute ordinary bash commands such as:
```console
mkdir foodPics
echo hello world
cat events
```
and compound commands (except parentheses).
```console
vim taxes; ls -a && cd.. || rm credits

```
However, _Rshell_ currently __does not support piping__. For example, the following line would fail:
```console
ls -a > myTextFile
```
Additionally, the _Rshell_ has its own built-in `EXIT` command for exiting the shell:
```console
EXIT
```

__NOTE:__ There are two Makefiles in our rshell repository. However, following the assignment specifications, there is only one contained within the Root directory. The other is inside /src/ and solely involves compiling the source files. The main Makefile, again within root, handles properly organizing the executable inside /bin. Our repository is set up so that the program will still build and run according to the specifications.


## Known Bugs
1. Parentheses and curly braces are not supported
2. Piping is not supported
3. Sometimes, `EXIT` won't work

## License
GNU General Public License v3.0

Copyright (C) 2017  Emilio Barreiro, Kevin Huang
