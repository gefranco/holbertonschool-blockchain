#include <stdio.h>
#include <stdlib.h>

#include "cli.h"

int save_cmd(state_t *state, int argc, char *argv[])
{

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

	if (blockchain_serialize(state->blockchain, argv[1]) == -1)
	{
		fprintf(stdout, "Failed to save blockchain to %s\n",
			argv[1]);
		return (state->status = EXIT_FAILURE);
	}

	fprintf(stdout, "Saved blockchain to %s\n", argv[1]);
	return ((state->status = EXIT_SUCCESS));
}
