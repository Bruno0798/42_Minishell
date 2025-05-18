# 42_Minishell

A custom shell implementation inspired by bash, developed as part of the 42 School curriculum.

## Project Description

Minishell is a simplified shell that replicates core functionality of bash. It processes user commands, manages processes, handles redirections and pipes, and includes several built-in commands. This project reinforces understanding of processes, file descriptors, signals, and environment management in Unix-like systems.

## Features Overview

- **Command Execution**: Run external programs with arguments
- **Command Line Parsing**: Support for quoting (both single `'` and double `"` quotes) 
- **Environment Variables**: Support for environment variable expansion (e.g., `$PATH`)
- **Exit Status**: Access to previous command's exit status via `$?`
- **Redirections**: Support for input (`<`), output (`>`), append (`>>`), and heredoc (`<<`)
- **Pipes**: Connect the output of one command to the input of another using `|`
- **Signals**: Handle signals like Ctrl+C (SIGINT), Ctrl+D (EOF), and Ctrl+\ (SIGQUIT)
- **Error Handling**: Comprehensive error messages and appropriate exit codes

## Built-in Commands

Minishell implements the following built-in commands:

| Command | Description |
|---------|-------------|
| `echo [-n]` | Display a line of text, with optional -n flag to omit trailing newline |
| `cd [directory]` | Change the current working directory |
| `pwd` | Print the current working directory |
| `export [name[=value]]` | Set environment variables |
| `unset [name]` | Remove environment variables |
| `env` | Display all environment variables |
| `exit [status]` | Exit the shell with optional status code |

## Installation & Usage

### Prerequisites

- GCC compiler
- Make
- Readline library

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/42_Minishell.git
   cd 42_Minishell
   ```

2. Compile the program:
   ```bash
   make
   ```

### Usage

Run the shell:
```bash
./minishell
```

## Technical Information

### Dependencies

- **Readline Library**: Used for command-line input handling with history navigation
- **Libft**: Custom C library implementing various utility functions

### Project Structure

The project is organized into several components:

- **Parser**: Processes the command line into tokens and validates syntax
- **Expander**: Handles variable expansion and quote removal
- **Executor**: Executes commands, manages pipes and redirections
- **Builtins**: Implements built-in shell commands
- **Environment**: Manages shell environment variables
- **Signals**: Handles signal interrupts

### Memory Management

The shell implements careful memory management to prevent leaks, using a combination of:
- Linked lists for dynamic data structures
- Proper cleanup functions for all allocated memory
- Systematic error checking and recovery
- `readline.supp` suppression file for Valgrind to handle readline's inherent memory leaks

### Norminette Compliance

This project follows the 42 School's Norminette coding standard which enforces:

- 25 lines maximum per function
- 80 characters maximum per line
- Only allowed functions from the subject
- Specific variable declaration and spacing rules
- Consistent indentation and formatting

All source code in this project passes the Norminette validator, ensuring clean, readable, and standardized code.

### Signal Handling

The shell handles the following signals:

- **SIGINT (Ctrl+C)**: Interrupts the current foreground process and displays a new prompt
- **SIGQUIT (Ctrl+\\)**: Sends a quit signal to the foreground process
- **EOF (Ctrl+D)**: Exits the shell when received at the prompt

Signal handling is context-aware with five distinct modes:

- **MAIN**: Default signal handling for the main shell process
- **CHILD**: Modified signal handling for child processes
- **HERE_DOC**: Special handling during here-document processing
- **PIPE**: Handling for piped commands
- **IGNORE**: Mode where SIGINT and SIGQUIT are explicitly ignored

This approach ensures appropriate signal behavior depending on the shell's current state.

## Implementation Details

- **Tokenization**: Custom lexer splits input into tokens while preserving quoted content
- **Command Execution**: Fork-based execution model for external commands
- **Redirection**: File descriptor management for input/output redirections
- **Pipes**: Process creation and file descriptor management for piped commands
- **Environment Management**: Custom implementation of environment variable storage and lookup

## Future Improvements

- Command history navigation
- Wildcard pattern expansion
- Job control capabilities
- Shell scripting features

## Contributors

- [Bruno Lopes](https://github.com/brpereir) - https://github.com/brpereiraa
- [Bruno Miguel](https://github.com/bsousa-d) - https://github.com/bruno0798

## License

This project is part of the 42 School curriculum and is not licensed for public use.
