#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <stdio.h>
#include "interpreter.h"

void skipLoop(char *program, size_t *ip)
{
	while(program[*ip++] != ']'){
		if(program[*ip] == '\0'){
			fprintf(stderr, "[ not closed");
			exit(SIGABRT);
		}
	}
}

void beginLoop(size_t *stack, size_t *stack_ptr, size_t *ip)
{
	*ip = stack[*stack_ptr];
}

void verify_integrity(size_t stack_ptr, size_t stack_size, size_t mem_ptr, size_t mem_size)
{
	if(stack_ptr > stack_size)
	{
		fprintf(stderr, "stack_ptr out of bounds, tried to access %zu but limit is %zu", stack_ptr, stack_size);
		exit(SIGSEGV);
	} else if(mem_ptr > mem_size)
	{
		fprintf(stderr, "mem_ptr out of bounds, tried to access %zu but limit is %zu", mem_ptr, mem_size);
		exit(SIGSEGV);
	}
}

void run(char* program, size_t mem_size, size_t stack_size)
{
	char memory[mem_size];
	size_t stack[stack_size];
	size_t ip, stack_ptr, mem_ptr = 0;

	char current_command = program[ip++];
	while(current_command != '\0')
	{
		switch(current_command){
			case '>':
				//mem_ptr++;
				mem_ptr = (mem_ptr + 1) % mem_size;
				break;

			case '<':
				//mem_ptr--;
				mem_ptr = (mem_ptr - 1) % mem_size;
				break;

			case '+':
				memory[mem_ptr]++;
				break;

			case '-':
				memory[mem_ptr]--;
				break;

			case ',':
				memory[mem_ptr] = getchar();
				break;

			case '.':
				putchar(memory[mem_ptr]);
				break;

			case '[':
				if (memory[mem_ptr] == 0){
					skipLoop(program, &ip);
				}else{
					stack[++stack_ptr] = ip;
				}
				break;

			case ']':
				if (memory[mem_ptr] != 0){
					beginLoop(stack, &stack_ptr, &ip);
				}else{
					stack_ptr = stack_ptr - 1;
				}
				break;

			case '#':
				scanf_s("%d", &memory[mem_ptr]);
				break;

			case '!':
				printf("%d", memory[mem_ptr]);
				break;
		}

		verify_integrity(stack_ptr, stack_size, mem_ptr, mem_size);

		current_command = program[ip++];
	}
}
