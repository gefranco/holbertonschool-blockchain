#include <llist.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cli.h"

/**
 * dispatch - dispatch a command by calling the associate function
 * @state: cli state
 * @argc: number of arguments passed
 * @argv: arguments
 * Return: -1 if not command found,
 *         otherwise the exit status of the executed command
 */
int dispatch(state_t *state, int argc, char *argv[])
{
	command_t *command = find_command(argv[0]);
	(void)argc;
	if (!command)
	{
		fprintf(stderr, "%s: command not found\n", argv[0]);
		return (-1);
	}
	return (command->func(state, argc, argv));
}

/**
 * tokenize - tokenize a line input
 * @input: the line to tokenize
 * @tkns: an array of strings to put the tokens
 * Return: the number of tokens
 */
int tokenize(char *input, char *tkns[])
{
	char *tk;
	int i;

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
	return (i);
}

/**
 * main - entry point
 * Return: EXIT_FAILURE if call to a function fails
 *         EXIT_SUCCESS if no command is executed
 *         otherwise the exit status of the las executed command
 */
int main(void)
{
	state_t state = {0};
	int status = 0;
	ssize_t nread = 0;
	char *line = NULL;
	char *tkns_input[100];
	int argc = 0;
	size_t line_n = 0;

	state_init(&state);
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
