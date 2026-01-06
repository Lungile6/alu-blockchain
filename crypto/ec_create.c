#include "hblk_crypto.h"

/**
 * ec_create - Creates a new EC key pair using the secp256k1 curve
 *
 * Return: Pointer to EC_KEY structure containing public/private keys,
 * or NULL upon failure.
 */
EC_KEY *ec_create(void)
{
	EC_KEY *key = NULL;

	/* 1. Create the key structure based on the curve NID */
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	/* 2. Generate the private and public key pair */
	if (!EC_KEY_generate_key(key))
	{
		EC_KEY_free(key);
		return (NULL);
	}

	return (key);
}
