# Pico Shell

## How does the shell work?

![Pico Shell](image-1.png)

Pico Shell is a simple command-line shell implemented to mimic basic functionalities found in Unix-like operating systems. The shell supports built-in commands like `echo`, `pwd`, `cd`, and `exit`, as well as features like command parsing and executing external programs. It also supports basic I/O redirection.

## Features and Implementation

### Built-in Commands
Pico Shell supports the following built-in commands:
- **echo**: Prints a string to the terminal.
- **pwd**: Prints the current working directory.
- **cd**: Changes the current directory.
- **exit**: Exits the shell.

### Command Line Parsing
The shell takes user input and parses it into individual arguments using `strtok`. The input is tokenized, and each token is processed as a separate command argument.

### External Command Execution
If the user enters a command other than the built-in ones, Pico Shell will execute the command using the `fork` and `execvp` system calls. This allows the shell to run external programs using their names directly, just as you would in a typical Unix shell.

### I/O Redirection
Pico Shell supports basic I/O redirection:
- **Input Redirection (`<`)**: Redirects input from a file to the program.
- **Output Redirection (`>`)**: Redirects output to a file.
- **Error Output Redirection (`2>`)**: Redirects error output to a file.

### Execution Flow
1. The user enters a command.
2. The command is parsed into arguments.
3. The shell checks if the command is a built-in one (like `echo`, `pwd`, `cd`, `exit`). If so, it executes the corresponding function.
4. If the command is not built-in, the shell forks a new process and executes the command using the `execvp` system call.
5. If I/O redirection is present, the shell sets up file descriptors for the redirection using `dup2` before executing the command.

