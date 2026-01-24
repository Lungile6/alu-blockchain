#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * serialize_in - Helper to copy transaction input data to buffer
 * @node: Pointer to the current tx_in_t node
 * @idx: Index of the node (unused)
 * @arg: Pointer to the current buffer address
 * Return: 0 (continue iteration)
 */
static int serialize_in(llist_node_t node, unsigned int idx, void *arg)
{
	tx_in_t *in = node;
	uint8_t **ptr = (uint8_t **)arg;

	(void)idx;

	memcpy(*ptr, in->block_hash, SHA256_DIGEST_LENGTH);
	*ptr += SHA256_DIGEST_LENGTH;
	memcpy(*ptr, in->tx_id, SHA256_DIGEST_LENGTH);
	*ptr += SHA256_DIGEST_LENGTH;
	memcpy(*ptr, in->tx_out_hash, SHA256_DIGEST_LENGTH);
	*ptr += SHA256_DIGEST_LENGTH;

	return (0);
}

/**
 * serialize_out - Helper to copy transaction output hashes to buffer
 * @node: Pointer to the current tx_out_t node
 * @idx: Index of the node (unused)
 * @arg: Pointer to the current buffer address
 * Return: 0 (continue iteration)
 */
static int serialize_out(llist_node_t node, unsigned int idx, void *arg)
{
	tx_out_t *out = node;
	uint8_t **ptr = (uint8_t **)arg;

	(void)idx;

	memcpy(*ptr, out->hash, SHA256_DIGEST_LENGTH);
	*ptr += SHA256_DIGEST_LENGTH;

	return (0);
}

/**
 * transaction_hash - Computes the ID (hash) of a transaction
 * @transaction: Points to the transaction to compute the hash of
 * @hash_buf: Buffer in which to store the computed hash
 *
 * Return: Pointer to hash_buf, or NULL on failure
 */
uint8_t *transaction_hash(transaction_t const *transaction,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t len;
	uint8_t *buffer, *ptr;

	if (!transaction || !hash_buf)
		return (NULL);

	/* Calculate total size: (inputs * 3 * 32) + (outputs * 32) */
	len = llist_size(transaction->inputs) * 3 * SHA256_DIGEST_LENGTH;
	len += llist_size(transaction->outputs) * SHA256_DIGEST_LENGTH;

	if (len == 0)
		return (NULL);

	buffer = malloc(len);
	if (!buffer)
		return (NULL);

	ptr = buffer;

	/* Iteratively copy data into the buffer */
	llist_for_each(transaction->inputs, serialize_in, &ptr);
	llist_for_each(transaction->outputs, serialize_out, &ptr);

	/* Hash the entire serialized buffer */
	if (!sha256((int8_t const *)buffer, len, hash_buf))
	{
		free(buffer);
		return (NULL);
	}

	free(buffer);
	return (hash_buf);
}
