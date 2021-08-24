#include <stdlib.h>
#include "blockchain.h"
/**
 * block_destroy - free the block
 * @block: block to free
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;
	llist_destroy(block->transactions, 1, (node_dtor_t)transaction_destroy);
	free(block);
}
