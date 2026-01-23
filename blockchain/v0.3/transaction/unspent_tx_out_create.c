#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * unspent_tx_out_create - Allocates and initializes an unspent tx output
 *
 * @block_hash: Hash of the Block containing the transaction
 * @tx_id:      ID of the transaction containing the output
 * @out:        Pointer to the transaction output
 *
 * Return: Pointer to the created unspent transaction output, or NULL
 */
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t const block_hash[SHA256_DIGEST_LENGTH],
	uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	tx_out_t const *out)
{
	unspent_tx_out_t *unspent_out;

	if (!block_hash || !tx_id || !out)
		return (NULL);

	unspent_out = malloc(sizeof(*unspent_out));
	if (!unspent_out)
		return (NULL);

	/* Copy the block hash and transaction ID for reference */
	memcpy(unspent_out->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(unspent_out->tx_id, tx_id, SHA256_DIGEST_LENGTH);

	/* Copy the actual output data (amount and public key) */
	memcpy(&unspent_out->out, out, sizeof(unspent_out->out));

	return (unspent_out);
}
