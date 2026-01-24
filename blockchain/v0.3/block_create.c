#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

/**
 * block_create - Creates a new Block structure
 * @prev: Pointer to the previous Block in the blockchain
 * @data: Data to store in the Block
 * @data_len: Length of the data to store
 *
 * Return: Pointer to the created Block, or NULL on failure
 */
block_t *block_create(block_t const *prev, int8_t const *data,
	uint32_t data_len)
{
	block_t *block;
	uint32_t len = data_len;

	block = malloc(sizeof(*block));
	if (!block)
		return (NULL);

	memset(block, 0, sizeof(*block));
	if (len > BLOCKCHAIN_DATA_MAX)
		len = BLOCKCHAIN_DATA_MAX;

	memcpy(block->data.buffer, data, len);
	block->data.len = len;

	block->info.index = prev->info.index + 1;
	block->info.timestamp = time(NULL);
	memcpy(block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);

	/* Initialize the transaction list for v0.3 */
	block->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!block->transactions)
	{
		free(block);
		return (NULL);
	}

	return (block);
}
