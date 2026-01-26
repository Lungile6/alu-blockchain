#include "blockchain.h"

/**
 * blockchain_difficulty - Computes the difficulty for the next Block
 * @blockchain: Pointer to the Blockchain to analyze
 *
 * Return: The difficulty to be assigned to a potential next Block
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t const *latest_block, *last_adjusted_block;
	uint64_t expected_time, actual_time;

	if (!blockchain)
		return (0);

	/* 1. Retrieve the latest block in the chain */
	latest_block = llist_get_tail(blockchain->chain);
	if (!latest_block)
		return (0);

	/* 2. Check if adjustment is needed (Multiple of interval and not Genesis) */
	if (latest_block->info.index == 0 ||
	    latest_block->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL != 0)
	{
		return (latest_block->info.difficulty);
	}

	/* 3. Retrieve the block from the last adjustment checkpoint */
	last_adjusted_block = llist_get_node_at(blockchain->chain,
		latest_block->info.index - DIFFICULTY_ADJUSTMENT_INTERVAL + 1);

	/* 4. Calculate time metrics */
	/* Expected time = interval (blocks) * generation_time (seconds/block) */
	expected_time = DIFFICULTY_ADJUSTMENT_INTERVAL * BLOCK_GENERATION_INTERVAL;
	/* Actual time = difference in timestamps between the two blocks */
	actual_time = latest_block->info.timestamp - last_adjusted_block->info.timestamp;

	/* 5. Adjustment Logic */
	/* If found too fast (Actual < Expected / 2), increase difficulty */
	if (actual_time < expected_time / 2)
	{
		return (latest_block->info.difficulty + 1);
	}
	/* If found too slow (Actual > Expected * 2), decrease difficulty */
	else if (actual_time > expected_time * 2)
	{
		if (latest_block->info.difficulty > 0)
			return (latest_block->info.difficulty - 1);
		return (0);
	}

	/* Otherwise, keep the current difficulty */
	return (latest_block->info.difficulty);
}
