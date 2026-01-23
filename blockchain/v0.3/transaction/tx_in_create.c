#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * tx_in_create - Allocates and initializes a transaction input structure
 *
 * @unspent: Pointer to the unspent transaction output to be
 * converted to an input
 *
 * Return: Pointer to the created transaction input, or NULL on failure
 */
tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *in;

	if (!unspent)
		return (NULL);

	in = malloc(sizeof(*in));
	if (!in)
		return (NULL);

	/* Store the reference to where the coins are coming from */
	memcpy(in->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(in->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(in->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);

	/* sig is initialized to zeros; it will be populated during signing later */
	memset(&in->sig, 0, sizeof(in->sig));

	return (in);
}
