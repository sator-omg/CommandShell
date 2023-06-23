# CommandShell
CommandShell is a lightweight and versatile command shell implemented in C++. This is a simple shell program implemented in C++. It allows you to execute various commands, change directories, create directories and files, edit files, and more.

## Features

- Change directory: `cd <directory>`
- Display help information: `help`
- Exit the shell: `exit`
- Display command history: `history`
- Create a directory: `mkdir <directory>`
- Create a file: `mkfile <filename>`
- Edit a file: `edit <filename>`
- Change background color: `bgcolor <color>`
- Change font color: `fgcolor <color>`
- List all directories and files: `lsall`

## Getting Started

To compile and run the shell program, follow these steps:

1. Clone the repository:

   ```shell
   git clone <repository_url>
2. Change to the project directory:
   ```shell
   cd shell-program
3. Compile the code:
   ```shell
   g++ shell.cpp -o shell
4. Run the shell program:
    ```shell
   ./shell

## Dependencies
The shell program has the following dependencies:

- C++ compiler
- `iostream` library
- `vector` library
- `string` library
- `cstring` library
- `cstdlib` library
- `unistd.h` library
- `sys/stat.h` library
- `sys/types.h` library
- `sys/wait.h` library
- `fstream` library
- `memory` library
