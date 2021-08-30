#include <llist.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cli.h"


int dispatch(state_t *state, int argc, char *argv[])
{
	command_t *command = find_command(argv[0]);
	(void)argc;
	if (!command)
	{
		fprintf(stderr, "%s: command not found\n", argv[0]);
		return -1;
	}
	return command->func(state, argc, argv);
}

int tokenize(char *input, char *tkns[])
{
	char *tk;
        int i;
	
	printf("%s\n", input);
        i = 0;
        tk = NULL;
        input[strlen(input) - 1] = '\0';
        tk = strtok(input, " ");

        while (tk)
        {
                tkns[i] = tk;
                tk = strtok(NULL, " ");
                i++;
        }
        tkns[i] = NULL;
	return i;
}


int main(void)
{
	state_t state = {0};
	int status = 0;
	ssize_t nread = 0;
	char *line = NULL;	
	char *tkns_input[100];
	int argc = 0;
	size_t line_n = 0;
	while (1)
	{
		printf(">");
		nread = getline(&line, &line_n, stdin);
		if (nread < 1)
		{
			printf("\n");
			break;
		}
		argc = tokenize(line, tkns_input);
		if (argc > 0)
			state.status = dispatch(&state, argc, tkns_input);
		
		argc = 0;
	}
	status = state.status;
	state_clear(&state);	
	return (status);
}
