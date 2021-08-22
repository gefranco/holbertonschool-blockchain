#include "transaction.h"
#include <string.h>
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *tx_out;
	
	if (!pub)
		return (NULL);
	tx_out = calloc(1, sizeof(tx_out_t));
	if (!tx_out)
		return(NULL);

	tx_out->amount = amount;
	memcpy(tx_out->pub, pub, EC_PUB_LEN);
	if (!sha256((const int8_t *)tx_out, SHA256_DIGEST_LENGTH, tx_out->hash))
	{
		free(tx_out);
		return (NULL);
	}
	return (tx_out);
}
