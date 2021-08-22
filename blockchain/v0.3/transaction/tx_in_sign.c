#include "transaction.h"
#include <string.h>


/**
 * tx_in_sign - signs a transaction input, given the transaction id it is from
 * @in: points to the transaction input structure to sign
 * @tx_id: contains the ID (hash) of the transaction input to sign is stored in
 * @sender: contains the private key of the sender
 * @all_unspent: is the list of all unspent transaction outputs to date
 * Return: a pointer to the resulting signature structure upon success
 *         or NULL upon failure
 */
sig_t *tx_in_sign(tx_in_t *in,
		uint8_t const tx_id[SHA256_DIGEST_LENGTH],
		EC_KEY const *sender, llist_t *all_unspent)
{
	uint8_t pub[EC_PUB_LEN];
	int i;
	unspent_tx_out_t *unspent_tx_out;

	for (i = 0; i < llist_size(all_unspent); i++)
	{
		unspent_tx_out = llist_get_node_at(all_unspent, i);
		if (memcmp(in->tx_out_hash,
			unspent_tx_out->out.hash,
			SHA256_DIGEST_LENGTH) == 0)
			break;
		unspent_tx_out = NULL;
	}

	if (!unspent_tx_out)
		return (NULL);
	if (!ec_to_pub(sender, pub))
		return (NULL);
	if (memcmp(pub, unspent_tx_out->out.pub, EC_PUB_LEN) != 0)
		return (NULL);
	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);

	return (&in->sig);
}
