#include <stdio.h>
#include <stdlib.h>

#include "cli.h"

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
