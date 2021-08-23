#include "transaction.h"
#include <string.h>

/**
 * sign_tx_in - Sign transaction input
 * @tx: current transaction input
 * @idx: index of transaction input
 * @args: argumnets
 *        arg[0]: tx_id
 *        arg[1]: sender private key
 *        arg[2]: all_unspent
 * Return: 0 on success, 1 on failure
 */
int sign_tx_in(llist_node_t tx, unsigned int idx, void *args)
{
	void **ptr = args;
	tx_in_t *tx_in = tx;
	(void) idx;
	if (!ec_sign(ptr[1], ptr[0], SHA256_DIGEST_LENGTH, &tx_in->sig))
		return (1);
	return (0);
}
/**
 * filter - collect sender's unspent transactions
 * @tx_out: current unspent transaction
 * @id_tx: index of unspent transaction output
 * @args: arguments
 *        arg[0]: sender's public key
 *        arg[1]: llist inputs
 *        arg[2]: amount
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int filter(llist_node_t tx_out, unsigned int id_tx, void *args)
{
	unspent_tx_out_t *unspent_tx = tx_out;
	tx_in_t *tx_in;
	void **p = args;
	llist_t *inputs = p[1];
	uint32_t *amount = p[2];

	(void)id_tx;
	if (memcmp(unspent_tx->out.pub, p[0], EC_PUB_LEN) == 0)
	{
		tx_in = tx_in_create(unspent_tx);
		llist_add_node(inputs, tx_in, ADD_NODE_REAR);
		*amount += unspent_tx->out.amount;
	}
	return (0);

}

static llist_t *out_txs_transfer(EC_KEY const *sender,
		EC_KEY const *receiver,
		uint32_t amount, uint32_t total_unspent)
{
	llist_t *outs;
	uint32_t leftover = total_unspent - amount;
	uint8_t pub[EC_PUB_LEN];
	tx_out_t *tx_out;

	outs = llist_create(MT_SUPPORT_FALSE);
	ec_to_pub(receiver, pub);
	tx_out = tx_out_create(amount, pub);

	llist_add_node(outs, tx_out, ADD_NODE_REAR);

	if (leftover > 0)
	{
		ec_to_pub(sender, pub);
		tx_out = tx_out_create(leftover, pub);
		llist_add_node(outs, tx_out, ADD_NODE_REAR);
	}

	return (outs);
}
/**
 * transaction_create - creates a transaction
 * @sender: contains the private key of the transaction sender
 * @receiver: contains the public key of the transaction receiver
 * @amount: the amount to send
 * @all_unspent: the list of all the unspent outputs to date
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
transaction_t *transaction_create(EC_KEY const *sender,
			EC_KEY const *receiver,
			uint32_t amount, llist_t *all_unspent)
{
	uint8_t pub[EC_PUB_LEN];
	transaction_t *tx;
	llist_t *inputs, *outputs;
	void *ptrs[3];
	uint32_t total_unspent;

	tx = calloc(1, sizeof(transaction_t));

	if (!tx)
		return (NULL);

	inputs = llist_create(MT_SUPPORT_FALSE);
	ec_to_pub(sender, pub);
	ptrs[0] = pub;
	ptrs[1] = inputs;
	ptrs[2] = &total_unspent;

	llist_for_each(all_unspent, filter, ptrs);

	if (total_unspent < amount)
	{
		free(tx);
		return (NULL);
	}
	tx->inputs = inputs;
	outputs = out_txs_transfer(sender, receiver, amount, total_unspent);
	tx->outputs = outputs;

	transaction_hash(tx, tx->id);

	ptrs[0] = tx->id;
	ptrs[1] = (void *)sender;
	ptrs[2] = all_unspent;
	llist_for_each(tx->inputs, sign_tx_in, ptrs);

	return (tx);
}
