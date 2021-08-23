#include "transaction.h"
#include <string.h>
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t  hash[SHA256_DIGEST_LENGTH];
	tx_in_t *tx_in;
	tx_out_t *tx_out;
	int i;

	if (!transaction_hash(coinbase, hash) || 
		memcmp(coinbase->id, hash, SHA256_DIGEST_LENGTH) !=0)
		return (0);
	if (llist_size(coinbase->inputs) != 1 ||
		llist_size(coinbase->outputs) != 1)
		return (0);
	tx_in = llist_get_node_at(coinbase->inputs, 0);
	tx_out = llist_get_node_at(coinbase->outputs, 0);
	
	if (memcmp(tx_in->tx_out_hash, &block_index, 4))
		return (0);
	
	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		if (tx_in->block_hash[i] || tx_in->tx_id[i])
			return (0);
	}
	for (i = 0; i < SIG_MAX_LEN; i++)
		if (tx_in->sig.sig[i])
			return (0);
	if (tx_out->amount != COINBASE_AMOUNT)
		return (0);
	
	return (1);
}
