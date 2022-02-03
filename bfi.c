#include <stdio.h>
#include "interpreter.h"

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("No parameters given");
		return 0;
	}

	size_t mem_size = 65535;
	size_t stack_size = 65535;
	char *program = argv[argc-1];

	run(program, mem_size, stack_size);

	return 0;
}
