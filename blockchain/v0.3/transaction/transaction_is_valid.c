#include "transaction.h"
#include <string.h>

/**
 * find_unspent_output - finds unspent output
 * @all_unspent: unspent tx ouput list
 * @tx_in: transaction input
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
unspent_tx_out_t *find_unspent_output(llist_t *all_unspent, tx_in_t *tx_in)
{
	int i, size;
	unspent_tx_out_t *utx_out;

	size = llist_size(all_unspent);
	for (i = 0; i < size; i++)
	{
		utx_out = llist_get_node_at(all_unspent, i);
		if (!memcmp(utx_out->out.hash,
			tx_in->tx_out_hash,
			sizeof(utx_out->out.hash) == 0) &&
			!memcmp(utx_out->block_hash,
				tx_in->block_hash,
				sizeof(utx_out->block_hash) == 0)
		)
			return (utx_out);
	}
	return (NULL);
}

int transaction_is_valid(transaction_t const *transaction,
		llist_t *all_unspent)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];
	int i, size;
	uint32_t total_unspent, total;
	unspent_tx_out_t *utx_out;
	tx_in_t *tx_in;
	tx_out_t *tx_out;

	total = 0;
	total_unspent = 0;
	if (!transaction ||
		!transaction_hash(transaction, hash) ||
		memcmp(hash, transaction->id, sizeof(hash))
	)
		return (0);

	size = llist_size(transaction->inputs);
	for (i = 0; i < size; i++)
	{
		tx_in = llist_get_node_at(transaction->inputs, i);
		utx_out = find_unspent_output(all_unspent, tx_in);

		if (!utx_out ||
			!ec_verify(ec_from_pub(utx_out->out.pub),
				transaction->id, SHA256_DIGEST_LENGTH, &tx_in->sig))
		{
			return (0);

		}
		total_unspent += utx_out->out.amount;

	}

	size = llist_size(transaction->outputs);

	for (i = 0; i < size; i++)
	{
		tx_out = (tx_out_t *)llist_get_node_at(transaction->outputs, i);
		total += tx_out->amount;
	}
	return (total_unspent == total);
}
