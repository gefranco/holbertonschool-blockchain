#include "blockchain.h"
#include <string.h>

/**
 * is_tx_invalid - validate wheter the transaction is valid or not
 * @node: transaction to validate
 * @idx: transaction index
 * @arg: arguments
 * Return: 0 valid 1 otherwise
 */
int is_tx_invalid(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *tx = node;
	valid_transaction_t *valid_tx = arg;

	if (idx == 0)
	{
		if (!coinbase_is_valid(tx, valid_tx->block_index))
			return (1);
	}
	else if (!transaction_is_valid(tx, valid_tx->all_unspent))
		return (1);

	return (0);
}
/**
 * is_valid_genesis - validate wheter the genesis block is valid or not
 * @block: block to validate
 * Return: 0 valid 1 otherwise
 */
int is_valid_genesis(const block_t *block)
{
	blockchain_t *blockchain = blockchain_create();
	int ret = 0;

	if (!blockchain)
		return (1);
	ret = memcmp(block, llist_get_head(blockchain->chain), sizeof(*block));
	blockchain_destroy(blockchain);
	if (ret != 0)
		return (1);
	return (0);
}

/**
 * block_is_valid - validate wheter the block is valid or not
 * @block: block to validate
 * @prev_block: previous block
 * @all_unspent: all unspent transactions
 * Return: 0 valid 1 otherwise
 */
int block_is_valid(block_t const *block,
		block_t const *prev_block, llist_t *all_unspent)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH] = {0};
	valid_transaction_t valid_tx = {0};


	if (!block)
		return (1);
	if (!prev_block)
		return (is_valid_genesis(block));
	if (block->info.index != prev_block->info.index + 1)
		return (1);
	if (!block_hash(prev_block, hash_buf) ||
		memcmp(hash_buf, prev_block->hash, SHA256_DIGEST_LENGTH))
		return (1);
	if (memcmp(prev_block->hash, block->info.prev_hash, SHA256_DIGEST_LENGTH))
		return (1);
	if (!block_hash(block, hash_buf) ||
		memcmp(hash_buf, block->hash, SHA256_DIGEST_LENGTH))
		return (1);
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);

	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);

	if (llist_size(block->transactions) < 1)
		return (1);

	valid_tx.is_valid = 1;
	valid_tx.all_unspent = all_unspent;
	valid_tx.block_index = block->info.index;

	if (llist_for_each(block->transactions, is_tx_invalid, &valid_tx))
		return (1);
	return (0);

}
