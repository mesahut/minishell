# Minishell Project

A simple shell implementation that mimics bash behavior.

## Project Structure

```
minishell/
├── src/                    # Source files
│   ├── main.c             # Program entry point
│   ├── lexer/             # Tokenization module
│   │   ├── lexer.c
│   │   └── lexer.h
│   ├── parser/            # Command parsing module
│   │   ├── parser.c
│   │   └── parser.h
│   ├── executor/          # Command execution module
│   │   ├── exec.c
│   │   └── exec.h
│   ├── builtins/          # Built-in commands
│   │   ├── builtin.c
│   │   ├── echo.c
│   │   ├── cd.c
│   │   ├── pwd.c
│   │   ├── export.c
│   │   ├── unset.c
│   │   ├── env.c
│   │   └── exit.c
│   ├── expansion/         # Variable expansion
│   │   ├── expand.c
│   │   └── expand.h
│   ├── environment/       # Environment management
│   │   ├── env.c
│   │   └── env.h
│   └── utils/             # Utility functions
│       ├── memory.c       # Memory management
│       ├── string.c       # String utilities
│       └── error.c        # Error handling
├── include/               # Header files
│   ├── minishell.h       # Main header
│   └── structures.h      # Data structures
├── libft/                 # Library functions
├── tests/                 # Test files
├── Makefile
└── README.md
```

## Features

- Command parsing and execution
- Built-in commands (echo, cd, pwd, export, unset, env, exit)
- Input/output redirection (<, >, >>)
- Environment variable expansion
- Pipeline support (|)
- Signal handling
- Memory management with garbage collection

## How to Compile

```bash
make
```

## How to Run

```bash
./minishell
```

## Supported Commands

- `echo [options] [string]` - Display text
- `cd [directory]` - Change directory
- `pwd` - Print working directory
- `export [name=value]` - Set environment variable
- `unset [name]` - Unset environment variable
- `env` - Display environment variables
- `exit [n]` - Exit shell
- External commands with PATH lookup 