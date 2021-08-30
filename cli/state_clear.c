#include "cli.h"

void state_clear(state_t *state)
{
	EC_KEY_free(state->wallet);
	state->wallet = NULL;
	blockchain_destroy(state->blockchain);
	state->blockchain = NULL;
}
