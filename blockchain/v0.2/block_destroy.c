#include <stdlib.h>
#include "blockchain.h"
/**
 * block_destroy - free the block
 * @block: block to free
 */
void block_destroy(block_t *block)
{
	free(block);
}
