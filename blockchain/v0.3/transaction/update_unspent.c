#include "transaction.h"
#include <string.h>

/**
 * find_unspent - find unspent transaction
 * @node: unspent tx
 * @hash: hash to compare
 * Return: 1 on success 0 otherwise
 */
int find_unspent(llist_node_t node, void *hash)
{
	unspent_tx_out_t *unspent = (unspent_tx_out_t *)node;

	return (memcmp(unspent->out.hash, hash, sizeof(unspent->out.hash)) == 0);
}


llist_t *update_unspent(llist_t *transactions,
	uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	int i, j, size, size_2;
	transaction_t *tx;
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	unspent_tx_out_t *unspent;

	size = llist_size(transactions);
	for (i = 0; i < size; i++)
	{
		tx = llist_get_node_at(transactions, i);
		size_2 = llist_size(tx->inputs);
		for (j = 0; j < size_2; j++)
		{
			tx_in = llist_get_node_at(tx->inputs, j);
			llist_remove_node(all_unspent, find_unspent, tx_in->tx_out_hash, 1, NULL);
		}
		size_2 = llist_size(tx->outputs);
		for (j = 0; j < size_2; j++)
		{
			tx_out = llist_get_node_at(tx->outputs, j);
			unspent = unspent_tx_out_create(block_hash, tx->id, tx_out);
			llist_add_node(all_unspent, unspent, ADD_NODE_REAR);
		}
	}

	return (all_unspent);
}
