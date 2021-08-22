#include "transaction.h"
#include <string.h>
/**
 * txin_to_buffer - copy transaction inputs into thee buffer
 * @tx_in: transaction input
 * @idx: index of the node
 * @buffer: buffer to copy transaction in
 * Return: 0 on success, 1 on failure
 */
int txin_to_buffer(llist_node_t tx_in, unsigned int idx, void *buffer)
{
	memcpy((int8_t *)buffer + idx
		* SHA256_DIGEST_LENGTH *
		3, tx_in, SHA256_DIGEST_LENGTH * 3);
	return (0);
}
/**
 * txout_to_buffer - copy transaction outputs into the buffer
 * @tx_out: transaction output
 * @idx: node index
 * @buffer: buffer to copy transaction in
 * Return: 0 on success, 1 on failure
 */
int txout_to_buffer(llist_node_t tx_out, unsigned int idx, void *buffer)
{
	memcpy((int8_t *)buffer +
		idx * SHA256_DIGEST_LENGTH,
		((tx_out_t *)tx_out)->hash,
		SHA256_DIGEST_LENGTH);
	return (0);
}
/**
 * transaction_hash - computes the ID (hash) of a transaction
 * @transaction: points to the transaction to compute the hash of
 * @hash_buf: is a buffer in which to store the computed hash
 * Return: a pointer to hash_buf
 */
uint8_t *transaction_hash(
		transaction_t const *transaction,
		uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{

	size_t tx_in_size, tx_out_size;
	int8_t *buffer;

	if (!transaction)
		return (NULL);

	tx_in_size = (size_t)llist_size(transaction->inputs) *
			SHA256_DIGEST_LENGTH * 3;
	tx_out_size = (size_t)llist_size(transaction->outputs) * SHA256_DIGEST_LENGTH;
	buffer = calloc(1, tx_in_size + tx_out_size);
	if (!buffer)
		return (NULL);

	llist_for_each(transaction->inputs, txin_to_buffer, buffer);
	llist_for_each(transaction->outputs, txout_to_buffer, buffer + tx_in_size);

	sha256(buffer, tx_in_size + tx_out_size, hash_buf);
	free(buffer);

	return (hash_buf);

}
