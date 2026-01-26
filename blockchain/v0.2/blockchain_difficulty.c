#include "blockchain.h"

/**
 * blockchain_difficulty - Computes the difficulty for the next Block
 * @blockchain: Pointer to the Blockchain to analyze
 *
 * Return: The difficulty to be assigned to a potential next Block
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t const *latest, *adjusted;
	uint64_t expected, actual;

	if (!blockchain)
		return (0);

	latest = llist_get_tail(blockchain->chain);
	if (!latest)
		return (0);

	if (latest->info.index == 0 ||
	    latest->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL != 0)
		return (latest->info.difficulty);

	/* Line 37 Fix: Adjusted index calculation to stay under 80 chars */
	adjusted = llist_get_node_at(blockchain->chain,
		latest->info.index - DIFFICULTY_ADJUSTMENT_INTERVAL + 1);

	expected = DIFFICULTY_ADJUSTMENT_INTERVAL * BLOCK_GENERATION_INTERVAL;
	actual = latest->info.timestamp - adjusted->info.timestamp;

	if (actual < expected / 2)
		return (latest->info.difficulty + 1);

	if (actual > expected * 2)
	{
		if (latest->info.difficulty > 0)
			return (latest->info.difficulty - 1);
		return (0);
	}

	return (latest->info.difficulty);
}
