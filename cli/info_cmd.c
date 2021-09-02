#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "cli.h"


static int sum_unspent(void *node, unsigned int idx, void *arg)
{
	unspent_tx_out_t *unspent_tx_out = node;
	uint32_t *total = arg;
	(void)idx;	
	*total += unspent_tx_out->out.amount;
	return (0);
}

int info_cmd(state_t *state, int argc, char *argv[])
{
	uint32_t coins = 0;
	
	if (argc > 1)
	{
		fprintf(stderr, "%s: too many arguments\n", argv[0]);
		return ((state->status = 2));
	}

	llist_for_each(state->blockchain->unspent, sum_unspent, &coins);
	
	fprintf(stdout, "Blocks:	%d\n",
		llist_size(state->blockchain->chain));
	
	fprintf(stdout, "Unspent transactions:	%d\n", 
		llist_size(state->blockchain->unspent));
	
	fprintf(stdout, "Transactions Pool:	%d\n",
		llist_size(state->tx_pool));
	
	fprintf(stdout, "Coins:	%"PRIu32"\n", coins);
	
	return ((state->status = EXIT_SUCCESS));
	
	
}
