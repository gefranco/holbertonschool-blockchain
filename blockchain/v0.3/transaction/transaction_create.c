#include "transaction.h"
#include <string.h>

int filter_unspent(llist_node_t node, unsigned int idx, void *args)
{
	unspent_tx_out_t *unspent = node;
	void **ptr = args;
	tx_in_t *tx_in;
	llist_t *inputs = ptr[1];
	uint32_t *amount = ptr[2];

	(void)idx;
	if (!memcmp(unspent->out.pub, ptr[0], EC_PUB_LEN))
	{
		tx_in = tx_in_create(unspent);
		llist_add_node(inputs, tx_in, ADD_NODE_REAR);
		*amount += unspent->out.amount;
	}
	return (0);
}

int sign_tr_in_action(llist_node_t node, unsigned int idx, void *args)
{
	void **ptr = args;
	tx_in_t *tx_in = node;

	(void)idx;
	if (!tx_in_sign(tx_in, ptr[0], ptr[1], ptr[2]))
	{
		return (1);
	}

	return (0);
}

llist_t *send_amount(EC_KEY const *sender,
					 EC_KEY const *receiver,
					 uint32_t amount, uint32_t total)
{
	llist_t *out;
	uint32_t leftover = total - amount;
	uint8_t pub[EC_PUB_LEN];
	tx_out_t *node;

	out = llist_create(MT_SUPPORT_FALSE);
	ec_to_pub(receiver, pub);
	node = tx_out_create(amount, pub);
	if (!node)
	{
		return (NULL);
	}
	llist_add_node(out, node, ADD_NODE_REAR);
	if (leftover != 0)
	{
		ec_to_pub(sender, pub);
		node = tx_out_create(leftover, pub);
		if (!node)
		{
			return (NULL);
		}
		llist_add_node(out, node, ADD_NODE_REAR);
	}

	return (out);
}

transaction_t *transaction_create(EC_KEY const *sender,
								  EC_KEY const *receiver,
								  uint32_t amount, llist_t *all_unspent)
{
	uint8_t pub[EC_PUB_LEN];
	transaction_t *tr;
	llist_t *in, *out;
	void *args[3];
	uint32_t unspent_amount = 0;

	if (!sender || !receiver || !all_unspent)
	{
		return (NULL);
	}
	tr = calloc(1, sizeof(*tr));
	if (!tr)
	{
		return (NULL);
	}
	in = llist_create(MT_SUPPORT_FALSE);
	ec_to_pub(sender, pub);
	args[0] = pub, args[1] = in, args[2] = &unspent_amount;
	llist_for_each(all_unspent, filter_unspent, args);
	if (unspent_amount < amount)
	{
		free(tr);
		return (NULL);
	}
	out = send_amount(sender, receiver, amount, unspent_amount);
	if (!out)
	{
		free(tr);
		return (NULL);
	}
	tr->inputs = in, tr->outputs = out;
	transaction_hash(tr, tr->id);
	args[0] = tr->id, args[1] = (void *)sender, args[2] = all_unspent;
	llist_for_each(tr->inputs, sign_tr_in_action, args);
	return (tr);
}
