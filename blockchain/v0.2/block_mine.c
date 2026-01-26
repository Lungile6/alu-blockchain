#include <string.h>
#include "blockchain.h"

/**
 * block_mine - Mines a Block by finding a valid nonce for a given difficulty
 * @block: Pointer to the Block to be mined
 */
void block_mine(block_t *block)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];

	if (!block)
		return;

	while (1)
	{
		block_hash(block, hash);

		if (hash_matches_difficulty(hash, block->info.difficulty))
		{
			memcpy(block->hash, hash, SHA256_DIGEST_LENGTH);
			break;
		}

		block->info.nonce++;
	}
}
