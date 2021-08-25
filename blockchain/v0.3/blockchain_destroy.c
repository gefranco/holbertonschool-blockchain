#include "blockchain.h"
#include <stdlib.h>
/**
 * blockchain_destroy - deletes the blockchain
 * @blockchain: blockchain to delete
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;
	llist_destroy(blockchain->chain, 1, NULL);
	llist_destroy(blockchain_unspent, 1, NULL);
	free(blockchain);
}
