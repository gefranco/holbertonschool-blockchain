#include <stdio.h>
#include <stdlib.h>

#include "cli.h"

int wallet_load(state_t *state, int argc, char *argv[100])
{
	EC_KEY *wallet = NULL;
	printf("loading...\n");	
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

	wallet = ec_load(argv[1]);	
	if (!wallet)
	{
		fprintf(stdout, "Failed to load wallet from %s\n",
			argv[1]);
		return ((state->status = EXIT_FAILURE));
	}

	EC_KEY_free(state->wallet);
	state->wallet = wallet;
	fprintf(stdout, "Loaded wallet from %s\n",
		argv[1]);
	
	return((state->status = EXIT_SUCCESS));
}
