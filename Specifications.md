# Environment
```c
const size_t program_size;
const size_t memory_size;
const size_t loop_stack_size;

char program[program_size];
char memory[memory_size] = {0};
char loops[loop_stack_size] = {0};

char *ip = program;
char *stack_ptr = loops;
char *mem_ptr = memory;
```


* `program` is the vector that contains the program
* `memory` is the vector that contains the modifiable data
* `loops` is a stack that stores `ip` positions of seen `[` when entered


* `ip` is the instruction pointer, of internal use
* `stack_ptr` is the loops pointer and should be always pointing at the top-most value, of internal use
* `mem_ptr` is the pointer of modifiable data, altered by the program


# BASIC COMMANDS
| Character | Specification                                          |
|-----------|--------------------------------------------------------|
| `>`       | `mem_ptr` incremented in 1                             |
| `<`       | `mem_ptr` decremented in 1                             |
| `+`       | `*mem_ptr` incremented in 1                            |
| `-`       | `*mem_ptr` decremented in 1                            |
| `,`       | stdin to `*mem_ptr`                                    |
| `.`       | `*mem_ptr` to stdout (ascii)                           |
| `[`       | if `*mem_ptr` is 0, then jumps to 1 after `]`          |
| `]`       | if `*mem_ptr` data is not 0, then jumps to 1 after `[` |


# EXTENDED COMMANDS
| Character | Specification                                     |
|-----------|---------------------------------------------------|
| `!`       | `*mem_ptr` to stdout (numerical representation)   |
| `#`       | stdin to `*mem_ptr` (numerical representation)    |


# Command expansion (WIP)
Although this language is not intended for serious programming, it has a QOL upgrade from standard brainfuck.
Command expansion abstracts the quantity of commands into a numerical representation directly before the command.
e.g.:
* `5>` would be expanded as `>>>>>`
* `3+` would be expanded as `+++`
