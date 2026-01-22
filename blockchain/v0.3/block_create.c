#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blockchain.h"

/**
 * block_create - Creates a Block structure and initializes it
 *
 * @prev: Pointer to the previous Block in the Blockchain
 * @data: Points to a memory area to duplicate in the Block’s data
 * @data_len: Number of bytes to duplicate in data
 *
 * Return: A pointer to the newly allocated Block, or NULL on failure
 */
block_t *block_create(block_t const *prev, int8_t const *data,
		      uint32_t data_len)
{
	block_t *new_block;
	uint32_t actual_len;

	if (!prev || !data)
		return (NULL);

	new_block = malloc(sizeof(*new_block));
	if (!new_block)
		return (NULL);

	/* Zero entire struct to clear padding, the hash field, and buffer */
	memset(new_block, 0, sizeof(*new_block));

	/* Initialize Block Info */
	new_block->info.index = prev->info.index + 1;
	new_block->info.difficulty = 0;
	new_block->info.timestamp = (uint64_t)time(NULL);
	new_block->info.nonce = 0;
	memcpy(new_block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);

	/* Initialize Block Data */
	actual_len = (data_len > BLOCKCHAIN_DATA_MAX) ?
		     BLOCKCHAIN_DATA_MAX : data_len;

	memcpy(new_block->data.buffer, data, actual_len);
	new_block->data.len = actual_len;

	/* new_block->hash is already 0 from memset */

	return (new_block);
}
