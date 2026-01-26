#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blockchain.h"

/**
 * block_create - Creates a new block
 * @prev: Pointer to the previous block
 * @data: Pointer to the data to be stored
 * @data_len: Length of the data to copy
 *
 * Return: Pointer to the new block, or NULL on failure
 */
block_t *block_create(block_t const *prev, int8_t const *data,
		      uint32_t data_len)
{
	block_t *block;
	uint32_t actual_len;

	if (!prev)
		return (NULL);

	block = calloc(1, sizeof(*block));
	if (!block)
		return (NULL);

	block->info.index = prev->info.index + 1;
	block->info.timestamp = (uint64_t)time(NULL);
	memcpy(block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);

	actual_len = data_len > BLOCKCHAIN_DATA_MAX ? BLOCKCHAIN_DATA_MAX : data_len;
	if (data && actual_len > 0)
	{
		memcpy(block->data.buffer, data, actual_len);
		block->data.len = actual_len;
	}

	return (block);
}
