#include "blockchain.h"
#include <string.h>

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
	block_t const _genesis = _genesis_block();

	if (!block)
		return (1);

	/* 1. Genesis Block Check */
	if (block->info.index == 0)
		return (memcmp(block, &_genesis, sizeof(_genesis)));

	if (!prev_block || block->info.index != prev_block->info.index + 1)
		return (1);

	/* 2. Previous Hash Consistency */
	if (memcmp(block->info.prev_hash, prev_block->hash, SHA256_DIGEST_LENGTH))
		return (1);

	/* 3. Proof of Work Check (v0.2 specific) */
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);

	/* 4. Hash Authenticity Check */
	block_hash(block, computed_hash);
	if (memcmp(block->hash, computed_hash, SHA256_DIGEST_LENGTH))
		return (1);

	return (0);
}
