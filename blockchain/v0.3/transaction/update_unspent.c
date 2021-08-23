#include "transaction.h"
#include <string.h>

int find_unspent(llist_node_t node, void *arg)
{
	unspent_tx_out_t *utxout = node;
	
	if (memcmp(arg, utxout->out.hash, SHA256_DIGEST_LENGTH))
		return 1;
	return (0);
}


int remove_unspent(llist_node_t node, unsigned int idx, void *args)
{
	void **ptrs_args = args;
	tx_in_t *txi = node;
	(void)idx;	
	llist_remove_node(ptrs_args[0], find_unspent, txi->tx_out_hash, 1, free);
	return (0);
}

int add_txo_to_unspent(llist_node_t node, unsigned int idx, void *args)
{
	void **ptrs_args = args;
	tx_out_t  *txo = node;
	unspent_tx_out_t *utxo;
	(void)idx;	
	utxo = unspent_tx_out_create(ptrs_args[1], ptrs_args[2], txo);
	
	if (!utxo)
	{
		fprintf(stderr, "fail to create an output");
		return (1);
	}
	if (llist_add_node(ptrs_args[0], utxo, ADD_NODE_REAR))
	{
		printf("fail to add unspent txo\n");
		return (1);
	}
	return (0);
}


int update_tx_unspent(llist_node_t node, unsigned int idx, void *args)
{
	transaction_t *tx = node;
	void *ptrs[3] = {0};
	void **ptrs_args = args;
	(void)idx;
	ptrs[0] = ptrs_args[0];
	ptrs[1] = ptrs_args[1];
	ptrs[2] = tx->id;
	
	llist_for_each(tx->inputs, remove_unspent, ptrs);
	llist_for_each(tx->outputs, add_txo_to_unspent, ptrs);	
	return (0);
}


llist_t *update_unspent(llist_t *transactions,
		uint8_t block_hash[SHA256_DIGEST_LENGTH],
		llist_t *all_unspent)
{
	
	void *args[2] = {0};
	args[0] = all_unspent;
	args[1] = block_hash;
	llist_for_each(transactions, update_tx_unspent, args);
	return (all_unspent);
}
