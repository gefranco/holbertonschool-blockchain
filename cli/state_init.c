#include "cli.h"

/**
 * state_init - initialize state structure
 * @state: cli state
 */
void state_init(state_t *state)
{
	state->wallet = ec_create();
	state->blockchain = blockchain_create();
	state->tx_pool = llist_create(MT_SUPPORT_FALSE);
}
