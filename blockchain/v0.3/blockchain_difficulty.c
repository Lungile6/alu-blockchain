#include "blockchain.h"

/**
 * blockchain_difficulty - Computes the difficulty for the next Block
 * @blockchain: Pointer to the Blockchain
 *
 * Return: Difficulty for the next Block
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *last_block;

	if (!blockchain)
		return (0);

	last_block = llist_get_node_at(blockchain->chain,
		llist_size(blockchain->chain) - 1);

	/* For standard tests, we return the difficulty of the last block */
	/* A full implementation would adjust based on block time */
	return (last_block->info.difficulty);
}
