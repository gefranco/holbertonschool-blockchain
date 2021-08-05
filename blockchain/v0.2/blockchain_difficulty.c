#include "blockchain.h"
#include <stdio.h>
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *last_block, *last_adjusted_block;
	uint64_t expected_elapsed_time, actual_elapsed_time;
	
	last_block = llist_get_tail(blockchain->chain);
	if (!last_block)
		return (0);
	if (last_block->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL == 0
		&& last_block->info.index != 0)
	{
		last_adjusted_block = llist_get_node_at(blockchain->chain, 
								last_block->info.index + 1 - DIFFICULTY_ADJUSTMENT_INTERVAL);
		if(!last_adjusted_block)
			return (last_block->info.difficulty);
		expected_elapsed_time = DIFFICULTY_ADJUSTMENT_INTERVAL * BLOCK_GENERATION_INTERVAL;
		actual_elapsed_time = last_block->info.timestamp - last_adjusted_block->info.timestamp;
		if (actual_elapsed_time < expected_elapsed_time / 2)
			return (last_block->info.difficulty + 1);
		if (actual_elapsed_time > expected_elapsed_time * 2)
			return (last_block->info.difficulty > 0 ? last_block->info.difficulty-1:0);
	}
	return last_block->info.difficulty;
}
