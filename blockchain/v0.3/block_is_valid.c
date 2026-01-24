#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

/**
 * block_is_valid - Verifies a block's validity including transactions
 * @block: Block to verify
 * @prev_block: Previous block in the chain
 * @all_unspent: List of unspent transaction outputs
 *
 * Return: 0 if valid, otherwise a non-zero value
 */
int block_is_valid(block_t const *block, block_t const *prev_block,
	llist_t *all_unspent)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];
	int i, tx_count;
	transaction_t *tx;

	if (!block || (!prev_block && block->info.index != 0))
		return (1);
	if (block->info.index == 0)
		return (memcmp(block->hash, GENESIS_HASH, SHA256_DIGEST_LENGTH));
	if (block->info.index != prev_block->info.index + 1)
		return (1);
	if (!block_hash(block, hash) ||
		memcmp(hash, block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);
	if (memcmp(block->info.prev_hash, prev_block->hash,
			SHA256_DIGEST_LENGTH) != 0)
		return (1);
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (1);

	/* v0.3: Transaction validation */
	tx_count = llist_size(block->transactions);
	if (tx_count <= 0)
		return (1);

	/* First transaction MUST be a valid coinbase */
	tx = llist_get_node_at(block->transactions, 0);
	if (!coinbase_is_valid(tx, block->info.index))
		return (1);

	/* Validate the rest of the transactions */
	for (i = 1; i < tx_count; i++)
	{
		tx = llist_get_node_at(block->transactions, i);
		if (!transaction_is_valid(tx, all_unspent))
			return (1);
	}
	return (0);
}
