#include <stdio.h>
#include <stdlib.h>

#include <llist.h>

#include "cli.h"

/**
 * transaction_is_invalid - find an invalid transaction
 *
 * @node: transaction
 * @arg: list of unspent transaction outputs
 *
 * Return: If @node is an invalid transaction, return 1.
 * Otherwise, return 0.
 */
static int transaction_is_invalid(llist_node_t node, void *arg)
{
	transaction_t *transaction = node;
	llist_t *unspent = arg;

	return (!transaction_is_valid(transaction, unspent));
}

/**
 * add_transaction - add a transaction to a block
 *
 * @node: transaction
 * @idx: index of node
 * @arg: block to which the transaction should be added
 *
 * Return: Upon adding transaction to block, return 0.
 * Upon failure, return -1.
 */
static int add_transaction(
	llist_node_t node, unsigned int idx __attribute__((unused)), void *arg)
{
	transaction_t *transaction = node;
	block_t *block = arg;

	return (llist_add_node(block->transactions, transaction, ADD_NODE_REAR));
}

/**
 * update_utxo - update UTXOs and create a new UTXO
 *
 * @state: CLI state
 * @block: mined block
 * @coinbase_tx: coinbase transaction
 *
 * Return: If an error occurs, return NULL.
 * Otherwise, return a pointer to a new unspent transaction output.
 */
static unspent_tx_out_t *update_utxo(
	state_t *state, block_t *block, transaction_t *coinbase_tx)
{
	state->blockchain->unspent = update_unspent(
		state->tx_pool, block->hash, state->blockchain->unspent);
	return (unspent_tx_out_create(
			block->hash, coinbase_tx->id,
			llist_get_head(coinbase_tx->outputs)));
}

/**
 * _cli_mine - mine a block (helper)
 *
 * @state: CLI state
 * @block: block to which the transaction pool should be added
 * @prev_block: previous block in the blockchain
 * @coinbase_tx: valid coinbase transaction to inject into new block
 *
 * Return: If an error occurs, return EXIT_FAILURE.
 * Otherwise, return EXIT_SUCCESS.
 */
static int _cli_mine(
	state_t *state, block_t *block, block_t *prev_block,
                transaction_t *coinbase_tx, char *argv[])
{
	unspent_tx_out_t *utxo = NULL;

	while (!llist_remove_node(
			state->tx_pool, transaction_is_invalid,
			state->blockchain->unspent, 1,
			(node_dtor_t)transaction_destroy))
		;
	llist_for_each(state->tx_pool, add_transaction, block);
	block->info.difficulty = blockchain_difficulty(state->blockchain);
	llist_add_node(block->transactions, coinbase_tx, ADD_NODE_FRONT);
	block_mine(block);
	if ((block_is_valid(block, prev_block, state->blockchain->unspent)) != 0)
	{
		fprintf(stderr, "%s: failed to mine a valid block\n",
			argv[0]);
		while (llist_pop(block->transactions))
			;
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}
	utxo = update_utxo(state, block, coinbase_tx);
	if (!utxo)
	{
		fprintf(stderr, "%s: failed to create UTXO\n",
			argv[0]);
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
	fprintf(stdout, "Successfully mined a block\n");
	return ((state->status = EXIT_SUCCESS));
}

/**
 * cli_mine - mine a block
 *
 * @state: CLI state
 *
 * Return: If called with the wrong number of arguments, return 2.
 * Otherwise, return EXIT_SUCCESS.
 */
int mine_cmd(state_t *state, int argc, char *argv[])
{
	int8_t block_data[BLOCKCHAIN_DATA_MAX] = {0};
	block_t *block = NULL;
	block_t *prev_block = llist_get_tail(state->blockchain->chain);
	transaction_t *coinbase_tx = NULL;

	if (argc > 1)
	{
		fprintf(stderr, "%s: too many arguments\n", argv[0]);
		return ((state->status = 2));
	}
	block = block_create(prev_block, block_data, BLOCKCHAIN_DATA_MAX);
	if (!block)
	{
		fprintf(stderr, "%s: failed to create block\n", argv[0]);
		return ((state->status = EXIT_FAILURE));
	}
	coinbase_tx = coinbase_create(state->wallet, block->info.index);
	if (!coinbase_tx)
	{
		fprintf(stderr, "%s: failed to create coinbase transaction\n",
			argv[0]);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}
	if (!coinbase_is_valid(coinbase_tx, block->info.index))
	{
		fprintf(stderr, "%s: failed to create valid coinbase transaction\n",
			argv[0]);
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}
	return (_cli_mine(state, block, prev_block, coinbase_tx, argv));
}
