#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

/**
 * block_hash - Computes the hash of a Block including its transactions
 * @block: Pointer to the Block to be hashed
 * @hash_buf: Buffer to store the resulting hash
 *
 * Return: Pointer to hash_buf
 */
uint8_t *block_hash(block_t const *block,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t size, offset;
	int i, tx_count;
	uint8_t *buf;
	transaction_t *tx;

	if (!block)
		return (NULL);
	tx_count = llist_size(block->transactions);
	if (tx_count < 0)
		tx_count = 0;

	size = sizeof(block->info) + block->data.len +
		(tx_count * SHA256_DIGEST_LENGTH);
	buf = malloc(size);
	if (!buf)
		return (NULL);

	/* Copy Info and Data */
	memcpy(buf, &block->info, sizeof(block->info));
	offset = sizeof(block->info);
	memcpy(buf + offset, block->data.buffer, block->data.len);
	offset += block->data.len;

	/* Copy each Transaction ID */
	for (i = 0; i < tx_count; i++)
	{
		tx = llist_get_node_at(block->transactions, i);
		memcpy(buf + offset, tx->id, SHA256_DIGEST_LENGTH);
		offset += SHA256_DIGEST_LENGTH;
	}

	sha256((int8_t const *)buf, size, hash_buf);
	free(buf);
	return (hash_buf);
}
