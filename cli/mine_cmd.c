#include <stdio.h>
#include <stdlib.h>
#include <llist.h>
#include "cli.h"


static int invalid_tx(llist_node_t node, void *arg)
{
	transaction_t *transaction = node;
	llist_t *unspent = arg;

	return (!transaction_is_valid(transaction, unspent));
}

static int add_transaction(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *transaction = node;
	block_t *block = arg;
	(void)idx;

	return (llist_add_node(block->transactions, transaction, ADD_NODE_REAR));
}

static int mine(state_t *state, block_t *block, block_t *prev_block,
		transaction_t *coinbase_tx, char *argv[])
{
	int size, i;
	unspent_tx_out_t *utxo = NULL;

	size = llist_size(state->tx_pool);
	for (i = 0; i < size; i++)
		llist_remove_node(state->tx_pool, invalid_tx, state->blockchain->unspent, 1,
				(node_dtor_t)transaction_destroy);

	llist_for_each(state->tx_pool, add_transaction, block);
	block->info.difficulty = blockchain_difficulty(state->blockchain);
	llist_add_node(block->transactions, coinbase_tx, ADD_NODE_FRONT);
	block_mine(block);
	if ((block_is_valid(block, prev_block, state->blockchain->unspent)) != 0)
	{
		fprintf(stderr, "%s: Failed to mine a valid block\n", argv[0]);
		while (llist_pop(block->transactions))
			;
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}
	state->blockchain->unspent = update_unspent(state->tx_pool,
					block->hash, state->blockchain->unspent);
	utxo = unspent_tx_out_create(block->hash, coinbase_tx->id,
				llist_get_head(coinbase_tx->outputs));
	if (!utxo)
	{
		fprintf(stderr, "%s failed to create unspent tx output\n", argv[0]);
		while (llist_pop(block->transactions))
			;
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}
	while (llist_pop(state->tx_pool))
		;
	llist_add_node(state->blockchain->chain, block, ADD_NODE_REAR);
	llist_add_node(state->blockchain->unspent, utxo, ADD_NODE_REAR);
	fprintf(stdout, "Block Successfuly mined\n");
	return ((state->status = EXIT_SUCCESS));
}


int mine_cmd(state_t *state, int argc, char *argv[])
{
	int8_t block_data[1024] = {0};

	block_t *block = NULL;
	block_t *prev_block = llist_get_tail(state->blockchain->chain);
	transaction_t *coinbase_tx = NULL;


	if (argc > 1)
	{
		fprintf(stderr, "%s: too many arguments\n", argv[0]);
		return ((state->status = 2));
	}

	block = block_create(prev_block, block_data, 1024);
	if (!block)
	{
		fprintf(stderr, "%s: failed to create block\n", argv[0]);
		return ((state->status = EXIT_FAILURE));
	}

	coinbase_tx = coinbase_create(state->wallet, block->info.index);
	if (!coinbase_tx)
	{
		fprintf(stderr, "%s: failed to create coinbase transaction\n", argv[0]);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}
	if (!coinbase_is_valid(coinbase_tx, block->info.index))
	{
		fprintf(stderr,
			"%s: failed to create valid coinbase transaction\n", argv[0]);
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}

	return (mine(state, block, prev_block, coinbase_tx, argv));
}
