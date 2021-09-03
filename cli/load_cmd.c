#include <stdio.h>
#include <stdlib.h>

#include "cli.h"

/**
 * load_cmd - load the blockchain from a file
 * @state: cli state
 * @argc: number of arguments passed
 * @argv: arguments
 * Return: 2 if number of arguments are wrong,
 *         EXIT_FAILURE if call to a function fails
 *         EXIT_SUCCESS otherwise
 */
int load_cmd(state_t *state, int argc, char *argv[])
{
	blockchain_t *blockchain = NULL;

	if (argc > 2)
	{
		fprintf(stderr, "%s: too many arguments\n", argv[0]);
		return ((state->status = 2));
	}

	if (argc < 2)
	{
		fprintf(stderr, "%s: too few arguments\n", argv[0]);
		return ((state->status = 2));
	}
	blockchain = blockchain_deserialize(argv[1]);

	if (!blockchain)
	{
		fprintf(stdout, "Failed to load blockchain from %s\n",
			argv[1]);
		return (state->status = EXIT_FAILURE);
	}
	blockchain_destroy(state->blockchain);
	state->blockchain = blockchain;
	fprintf(stdout, "Loaded blockchain from %s\n", argv[1]);
	return ((state->status = EXIT_SUCCESS));
}
