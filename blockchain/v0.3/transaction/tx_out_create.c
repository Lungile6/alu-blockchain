#include <stdlib.h>
#include <string.h>
#include "transaction.h"
#include "hblk_crypto.h"

/**
 * tx_out_create - Allocates and initializes a transaction output structure
 *
 * @amount: Amount of the transaction
 * @pub: Public key of the transaction receiver
 *
 * Return: Pointer to the created transaction output, or NULL on failure
 */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *out;
	uint8_t hash_buf[sizeof(amount) + EC_PUB_LEN];

	if (!pub)
		return (NULL);

	out = malloc(sizeof(*out));
	if (!out)
		return (NULL);

	out->amount = amount;
	memcpy(out->pub, pub, EC_PUB_LEN);

	/* Prepare buffer for hashing: Concatenate amount and public key */
	memcpy(hash_buf, &amount, sizeof(amount));
	memcpy(hash_buf + sizeof(amount), pub, EC_PUB_LEN);

	/* Cast hash_buf to (int8_t const *) to match sha256 prototype */
	if (!sha256((int8_t const *)hash_buf, sizeof(hash_buf), out->hash))
	{
		free(out);
		return (NULL);
	}

	return (out);
}
