#include "transaction.h"
#include <string.h>
unspent_tx_out_t *unspent_tx_out_create(
		uint8_t block_hash[SHA256_DIGEST_LENGTH],
		uint8_t tx_id[SHA256_DIGEST_LENGTH],
		tx_out_t const *out)
{
	unspent_tx_out_t *unspent_tx_out;

	unspent_tx_out = calloc(1, sizeof(unspent_tx_out_t));
	if (!unspent_tx_out)
		return (NULL);
	memcpy(unspent_tx_out->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(unspent_tx_out->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	memcpy(&unspent_tx_out->out, out, sizeof(*out));


	return (unspent_tx_out);
}
