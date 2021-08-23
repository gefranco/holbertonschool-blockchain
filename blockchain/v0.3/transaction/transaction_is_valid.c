#include "transaction.h"
#include <string.h>
int match_unspent(llist_node_t unspent, void *arg)
{
	unspent_tx_out_t *unspent_out = unspent;
	tx_in_t *tx_in = arg;

	if (memcmp(tx_in->tx_out_hash,
			unspent_out->out.hash,
			SHA256_DIGEST_LENGTH) == 0)
		return (1);
	return (0);
}

int exist_unspent_output(llist_node_t tx_input, unsigned int idx, void *arg)
{
	tx_in_t *tx_in = tx_input;
	valid_transaction_t *valid_tx = arg;
	unspent_tx_out_t *unspent_tx_out = llist_find_node(
						valid_tx->all_unspent, match_unspent, tx_in);

	EC_KEY *key;

	(void)idx;
	if (!unspent_tx_out)
	{
		valid_tx->is_valid = 0;
		printf("no utxo");
		return (1);
	}
	key = ec_from_pub(unspent_tx_out->out.pub);
	if (!key || !ec_verify(key,
				valid_tx->tx->id,
				SHA256_DIGEST_LENGTH,
				&tx_in->sig))
	{
		valid_tx->is_valid = 0;
		EC_KEY_free(key);
		printf("invalid sign");
		return (1);
	}

	EC_KEY_free(key);
	valid_tx->in_amount += unspent_tx_out->out.amount;
	return (0);

}

int sum_output(llist_node_t tx_out, unsigned int idx, void *arg)
{
	tx_out_t *tx = tx_out;
	valid_transaction_t *valid_tx = arg;

	(void)idx;
	valid_tx->out_amount += tx->amount;
	return (0);
}

int transaction_is_valid(transaction_t const *transaction,
		llist_t *all_unspent)
{
	uint8_t tx_hash[SHA256_DIGEST_LENGTH];

	valid_transaction_t valid_transaction = {0};

	valid_transaction.tx = transaction;
	valid_transaction.all_unspent = all_unspent;
	valid_transaction.is_valid = 1;
	valid_transaction.in_amount = 0;
	valid_transaction.out_amount = 0;

	transaction_hash(transaction, tx_hash);

	if (memcmp(transaction->id, tx_hash, SHA256_DIGEST_LENGTH) != 0)
	{
		printf("Invalid hash");
		return (0);
	}
	if (llist_for_each(transaction->inputs,
				exist_unspent_output,
				&valid_transaction))
		return (0);

	llist_for_each(transaction->outputs, sum_output, &valid_transaction);

	if (valid_transaction.in_amount != valid_transaction.out_amount)
	{
		return (0);
	}
	return (1);
}
