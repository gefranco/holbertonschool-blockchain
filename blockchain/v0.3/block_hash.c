#include "blockchain.h"
#include <stdlib.h>
#include <string.h>
#include "../../crypto/hblk_crypto.h"

/**
 * tx_to_buf - copy transaction to buf
 * @tx: the transaction
 * @idx: index transaction
 * @buffer: the buffer to copy in
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
static int tx_to_buf(llist_node_t tx, unsigned int idx, void *buffer)
{
	memcpy((int8_t *)buffer + idx * SHA256_DIGEST_LENGTH,
		((transaction_t *)tx)->id, SHA256_DIGEST_LENGTH);
	return (0);
}
uint8_t *block_hash(block_t const *block,
			uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t len;
	int list_size;
	int8_t *buffer;


	len = sizeof(block->info) + block->data.len;
	list_size = llist_size(block->transactions);
	if (list_size > 0)
		len += list_size * SHA256_DIGEST_LENGTH;

	buffer = calloc(1, len);
	if (!buffer)
		return (NULL);

	memcpy(buffer, block, sizeof(block->info) + block->data.len);

	llist_for_each(block->transactions, tx_to_buf,
			buffer + sizeof(block->info) + block->data.len);

	sha256(buffer, len, hash_buf);
	free(buffer);

	return (hash_buf);
}
