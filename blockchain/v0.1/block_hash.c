#include <stdlib.h>
#include "blockchain.h"
#include "hblk_crypto.h"

/**
 * block_hash - Computes the hash of a Block
 *
 * @block: Pointer to the Block to be hashed
 * @hash_buf: Buffer where the resulting hash must be stored
 *
 * Return: A pointer to hash_buf
 */
uint8_t *block_hash(block_t const *block,
		    uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t len;

	if (!block || !hash_buf)
		return (NULL);

	/**
	     * Calculate length: block_info_t size + the actual data length stored.
	 * Since 'info' and 'data' are the first two fields in block_t,
	 * we hash from the start of the block.
	 */
	len = sizeof(block->info) + block->data.len;

	/* sha256 function is provided in the hblk_crypto library */
	if (!sha256((int8_t const *)block, len, hash_buf))
		return (NULL);

	return (hash_buf);
}
