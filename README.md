# BrainFuck Interpreter

Live interpreter of brainfuck programming language


For details in language specificiation see [specifications](Specifications.md)


# Command use
```
bfi [FLAGS] {FILENAME}

FLAGS:
  --mem-size -m: indicates the reserved manipulable memory space in bytes (default is 65535)
  --loop-size -l: indicates the reserved stack size in bytes (default is 65535)
  --step -s: if this flag is set, the program will need user input to cycle through instructions
  --information -i: prints useful information each cycle (such as, current cell value, current pointer)
  --help -h: prints this
```
