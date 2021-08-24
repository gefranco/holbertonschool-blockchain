#include "blockchain.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
block_t *block_create(block_t const *prev,
			int8_t const *data, uint32_t data_len)
{

	block_t *block;
	uint32_t max_len = data_len > BLOCKCHAIN_DATA_MAX ?
		   BLOCKCHAIN_DATA_MAX : data_len;

	if (!prev || !data)
		return (NULL);
	block = calloc(1, sizeof(*block));
	if (!block)
		return (NULL);
	block->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!block->transactions)
		return (free(block), NULL);
	memcpy(block->data.buffer, data, max_len);
	block->data.len = max_len;

	memcpy(block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);
	block->info.index = prev->info.index + 1;
	block->info.timestamp = (uint64_t)time(NULL);

	return (block);
}
