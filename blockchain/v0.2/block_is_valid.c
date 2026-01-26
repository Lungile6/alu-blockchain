#include <string.h>
#include "blockchain.h"

/**
 * block_is_valid - Verifies if a Block is valid
 * @block: Pointer to the Block to check
 * @prev_block: Pointer to the previous Block in the chain
 *
 * Return: 0 if valid, otherwise 1 if invalid
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t computed_hash[SHA256_DIGEST_LENGTH];

	if (!block)
		return (1);

	/* 1. Genesis Block Check (Index 0) */
	if (block->info.index == 0)
	{
		/* Compare against the extern _genesis variable */
		return (memcmp(block, &_genesis, sizeof(_genesis)) != 0);
	}

	/* 2. Basic Chain Integrity */
	if (!prev_block || block->info.index != prev_block->info.index + 1)
		return (1);

	if (memcmp(block->info.prev_hash, prev_block->hash,
			SHA256_DIGEST_LENGTH) != 0)
		return (1);

	/* 3. Proof of Work Check (v0.2) */
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);

	/* 4. Hash Consistency Check */
	block_hash(block, computed_hash);
	if (memcmp(block->hash, computed_hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);

	return (0);
}
