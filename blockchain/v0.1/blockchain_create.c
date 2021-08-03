#include "blockchain.h"
#include <stdlib.h>
#include <string.h>

#define HASH "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03" /* c52c26c8b5461639635d8edf2a97d48d0c8e0009c817f2b1d3d7ff2f04515803 */
#define TIMESTAMP 1537578000

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
	memcpy(block->hash, HASH, SHA256_DIGEST_LENGTH);
	
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
