#include "blockchain.h"
#include <stdlib.h>
#include <string.h>

#define TIMESTAMP 1537578000
/**
 * blockchain_create - creates a Blockchain structure, and initializes it
 *
 * Return: a new Blockchain or NULL
 */
blockchain_t *blockchain_create(void)
{




	blockchain_t *blockchain = NULL;
	block_t *block = NULL;
	llist_t *list = NULL;

	blockchain = calloc(1, sizeof(blockchain_t));
	block = calloc(1, sizeof(block_t));
	list = llist_create(MT_SUPPORT_TRUE);

	if (!blockchain || !block || !list)
	{
		free(blockchain);
		free(block);
		llist_destroy(list, 1, NULL);
		return (NULL);
	}
	block->info.timestamp = TIMESTAMP;
	memcpy(block->hash, GENESIS_HASH, SHA256_DIGEST_LENGTH);

	memcpy(block->data.buffer, "Holberton School", 16);
	block->data.len = 16;

	if (llist_add_node(list, block, ADD_NODE_FRONT) == -1)
	{
		free(blockchain);
		free(block);
		llist_destroy(list, 1, NULL);
		return (NULL);
	}

	blockchain->chain = list;

	return (blockchain);

}
