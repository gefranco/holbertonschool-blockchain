#include "blockchain.h"
#include <stdlib.h>
#include "../../crypto/hblk_crypto.h"
uint8_t *block_hash(block_t const *block,
			uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t len = sizeof(block->info) + sizeof(block->data.len);

	return (sha256((int8_t *)block, len, hash_buf));
}
