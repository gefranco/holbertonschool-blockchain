#include "blockchain.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
block_t *block_create(block_t const *prev,
			int8_t const *data, uint32_t data_len)
{
	block_t *block;
	uint32_t len;

	block = calloc(1, sizeof(block_t));

	if (!block || !prev || !data)
	{
		return (NULL);
	}

	len = data_len > BLOCKCHAIN_DATA_MAX ?
		BLOCKCHAIN_DATA_MAX : data_len;

	memcpy(block->data.buffer, data, len);
	block->data.len = len;

	memcpy(block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);
	block->info.index = prev->info.index + 1;
	block->info.timestamp = (uint64_t)time(NULL);
	return (block);
}
