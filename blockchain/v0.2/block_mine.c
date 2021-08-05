#include "blockchain.h"
#include <stdio.h>
void block_mine(block_t *block)
{
	uint64_t nonce = 0;
	do
	{
		block->info.nonce = nonce;
		block_hash(block, block->hash);
		nonce ++;
	}while (!hash_matches_difficulty(block->hash,block->info.difficulty));
}
