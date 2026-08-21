# jsonparser

A lightweight, memory-safe JSON lexer and recursive-descent parser written in C.

## Features

* Full JSON support: objects, arrays, strings, numbers, booleans, and null
* Dynamic buffer resizing for tokens, key-value pairs, and elements
* Tagged union memory representation for AST nodes
* Recursive cleanup routines with zero memory leaks

## Project Structure

* `main.c`: CLI entry point and test runner
* `tokenizer.h` / `tokenizer.c`: Lexer and token definitions
* `parser.h` / `parser.c`: AST nodes, recursive-descent parser, and memory management

## Compilation

```bash
gcc -Wall -Wextra -g main.c tokenizer.c parser.c -o jsonparser
```

## Usage

Run against any JSON file:

```bash
./jsonparser test.json
```

Verify memory safety:

```bash
valgrind --leak-check=full ./jsonparser test.json
```
