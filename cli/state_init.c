#include "cli.h"

void state_init(state_t *state)
{
	state->wallet = ec_create();
	state->blockchain = blockchain_create();
	
}
