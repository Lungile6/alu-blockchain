#include <string.h>
#include "blockchain.h"

/**
 * block_is_valid - Verifies that a Block is valid
 * @block: Pointer to the Block to check
 * @prev_block: Pointer to the previous Block, or NULL if block is index 0
 *
 * Return: 0 if valid, otherwise 1
 */
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t computed_hash[SHA256_DIGEST_LENGTH];
	block_t const genesis = {
		{ 0, 0, 1537578000, 0, {0} },
		{ "Holberton School", 16 },
		"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"
		"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"
	};

	if (!block || (!prev_block && block->info.index != 0))
		return (1);

	/* Check Genesis Block */
	if (block->info.index == 0)
		return (memcmp(block, &genesis, sizeof(genesis)));

	/* Check Index sequence */
	if (block->info.index != prev_block->info.index + 1)
		return (1);

	/* Check data length */
	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);

	/* Verify prev_block's stored hash matches its actual content */
	if (!block_hash(prev_block, computed_hash) ||
	    memcmp(computed_hash, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);

	/* Verify block's reference to prev_block matches prev_block's hash */
	if (memcmp(prev_block->hash, block->info.prev_hash,
		   SHA256_DIGEST_LENGTH) != 0)
		return (1);

	/* Verify current block's stored hash matches its actual content */
	if (!block_hash(block, computed_hash) ||
	    memcmp(computed_hash, block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);

	return (0);
}
