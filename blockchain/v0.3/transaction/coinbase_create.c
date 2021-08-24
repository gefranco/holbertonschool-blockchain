#include "transaction.h"
#include <string.h>
/**
 * coinbase_create - function that creates a coinbase transaction
 * @receiver: contains the public key of the miner
 * @block_index: is the index of the Block the
 *               coinbase transaction will belong to
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *tx;
	tx_out_t *tx_out;
	tx_in_t *tx_in;
	uint8_t pub[EC_PUB_LEN];

	if (!ec_to_pub(receiver, pub))
		return (NULL);

	tx_out = tx_out_create(COINBASE_AMOUNT, pub);
	if (!tx_out)
		return (NULL);
	tx_in = calloc(1, sizeof(tx_in_t));

	if (!tx_in)
	{
		free(tx_out);
		return (NULL);
	}
	memcpy(tx_in->tx_out_hash, &block_index, sizeof(block_index));

	tx = calloc(1, sizeof(transaction_t));
	if (!tx)
	{
		free(tx_in);
		free(tx_out);
		return (NULL);
	}

	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	llist_add_node(tx->inputs, tx_in, ADD_NODE_REAR);
	llist_add_node(tx->outputs, tx_out, ADD_NODE_REAR);

	transaction_hash(tx, tx->id);
	return (tx);
}
