#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MEM_SIZE 30000
#define PROGRAM_SIZE 3000

typedef struct SInterpreter {
  int programCounter;
  int dataPointer;
  char *memory;
  char *program;
} SInterpreter;

/**
 * @return 0 if initialization was successful, 1 if an allocation problem arose
 * @note Prints error if allocation fails
 */
int initializeInterpreter(SInterpreter *interpreter) {
  interpreter->dataPointer = 0;
  interpreter->programCounter = 0;
  interpreter->program = (char *)malloc(PROGRAM_SIZE * sizeof(char));
  interpreter->memory = (char *)malloc(MEM_SIZE * sizeof(char));

  if (!interpreter->program || !interpreter->memory) {
    printf("COULDN'T ALLOCATE MEMORY FOR INTERPRETER\n");
    return 1;
  }

  for (int i = 0; i < MEM_SIZE; ++i) {
    interpreter->memory[i] = 0;
  }

  return 0;
}

void disposeInterpreter(SInterpreter *interpreter) {
  free(interpreter->program);
  free(interpreter->memory);
}

/**
 * Reads the entire contents of a file into a dynamically allocated string.
 *
 * @param path The path to the file to read
 * @return Pointer to allocated buffer containing file contents, or NULL on
 * failure
 * @note The caller is responsible for freeing the returned buffer
 */
char *get_file(char const *path) {
  char *buffer = 0;
  size_t bufLength;

  FILE *f = fopen(path, "r");
  if (!f) {
    fprintf(stderr, "COULDN'T OPEN FILE %s\n", path);
    return NULL;
  }

  fseek(f, 0, SEEK_END);
  bufLength = ftell(f);
  fseek(f, 0, SEEK_SET);

  buffer = (char *)malloc((bufLength + 1) * sizeof(char));
  if (buffer) {
    size_t bytesRead = fread(buffer, sizeof(char), bufLength, f);
    if (bytesRead != bufLength) {
      fprintf(stderr,
              "ERROR READING FILE: Expected %zu bytes, but got %zu instead\n",
              bufLength, bytesRead);
      free(buffer);
      fclose(f);
      return NULL;
    }
    buffer[bufLength] = '\0';
  }

  fclose(f);
  return buffer;
}

/**
 * Finds the matching opening bracket '[' for a given closing bracket ']'
 * position. Searches backward through the program to find the corresponding
 * bracket.
 *
 * @param program The Brainfuck program string
 * @param position The index of the closing bracket ']' to find match for
 * @return Index of the matching opening bracket '[', or -1 if not found
 * @note Prints error message to stderr if no matching bracket is found
 */
int find_matching_open(char *program, int position) {
  size_t depth = 1;
  int err_pos = position;
  position--;

  while (position >= 0) {
    char operation = program[position];
    if (operation == ']')
      depth++;
    else if (operation == '[')
      depth--;

    if (depth == 0)
      return position;

    position--;
  }

  fprintf(stderr, "NO OPENING PAIR: MISSING '[' @pos=%i\n", err_pos);
  return -1;
}

/**
 * Finds the matching closing bracket ']' for a given opening bracket '['
 * position. Searches forward through the program to find the corresponding
 * bracket.
 *
 * @param program The Brainfuck program string
 * @param position The index of the opening bracket '[' to find match for
 * @return Index of the matching closing bracket ']', or -1 if not found
 * @note Prints error message to stderr if no matching bracket is found
 */
int find_matching_close(char *program, int position) {
  size_t depth = 1;
  int err_pos = position;
  ++position;

  char operation = program[position];
  while (operation != '\0') {
    if (operation == '[')
      depth++;
    else if (operation == ']')
      depth--;

    if (depth == 0)
      return position;

    ++position;
    operation = program[position];
  }

  fprintf(stderr, "NO CLOSING PAIR: MISSING ']' @pos=%i\n", err_pos);
  return -1;
}

int run(SInterpreter *interpreter, char *program) {

  char operation = program[interpreter->programCounter];
  while (operation != '\0') {
    switch (operation) {
    case '>':
      interpreter->dataPointer = (interpreter->dataPointer + 1) % MEM_SIZE;
      break;
    case '<':
      interpreter->dataPointer =
          (interpreter->dataPointer - 1 + MEM_SIZE) % MEM_SIZE;
      break;
    case '+':
      interpreter->memory[interpreter->dataPointer]++;
      break;
    case '-':
      interpreter->memory[interpreter->dataPointer]--;
      break;
    case '.':
      putchar(interpreter->memory[interpreter->dataPointer]);
      fflush(stdout);
      break;
    case ',': {
      char c = getchar();
      interpreter->memory[interpreter->dataPointer] = (c == EOF) ? 0 : c;
    } break;

    case '[':
      if (interpreter->memory[interpreter->dataPointer] == 0) {
        interpreter->programCounter =
            find_matching_close(program, interpreter->programCounter);
        if (interpreter->programCounter == -1) {
          free(program);
          return EXIT_FAILURE;
        }
      }
      break;

    case ']':
      if (interpreter->memory[interpreter->dataPointer] != 0) {
        interpreter->programCounter =
            find_matching_open(program, interpreter->programCounter);
        if (interpreter->programCounter == -1) {
          free(program);
          return EXIT_FAILURE;
        }
      }
      break;
    default:
      break;
    }
    operation = program[++interpreter->programCounter];
  }
  return EXIT_SUCCESS;
}

/**
 * @note Memory layout: 30,000 bytes of memory (as per standard Brainfuck)
 * @note Implements all 8 Brainfuck operations: > < + - . , [ ]
 * @note Handles pointer wrapping and bracket matching automatically
 * @note If no filename provided, reads program from standard input
 */
int main(int argc, char *argv[]) {
  char *program;
  SInterpreter interpreter;
  int status = initializeInterpreter(&interpreter);
  if (status != 0)
    return EXIT_FAILURE;

  if (argc == 2) {
    program = get_file(argv[1]);
    if (!program)
      return EXIT_FAILURE;
  } else {
    program = (char *)malloc(PROGRAM_SIZE * sizeof(char));
    if (!program) {
      fprintf(stderr, "COULDN'T ALLOCATE MEMORY\n");
      return EXIT_FAILURE;
    }

    int i = 0;
    int c;
    while ((c = getchar()) != '\n' && c != EOF && i < PROGRAM_SIZE - 1) {
      program[i++] = c;
    }
    program[i] = '\0';

    if (i == 0 && c == EOF) {
      fprintf(stderr, "Error reading input\n");
      free(program);
      return EXIT_FAILURE;
    }
  }
  printf("Program: %s\n", program);
  fflush(stdout);

  run(&interpreter, program);

  printf("\nPROGRAM FINISHED --- CLOSING\n");
  disposeInterpreter(&interpreter);
  free(program);

  return EXIT_SUCCESS;
}
