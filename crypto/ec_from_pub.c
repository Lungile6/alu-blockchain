#include "hblk_crypto.h"

/**
 * ec_from_pub - Creates an EC_KEY structure given a public key
 * @pub: Contains the public key to be converted (uncompressed)
 *
 * Return: Pointer to the created EC_KEY structure, or NULL upon failure
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key = NULL;
	EC_POINT *point = NULL;
	const EC_GROUP *group = NULL;

	if (!pub)
		return (NULL);

	/* 1. Create a new EC_KEY structure for the correct curve */
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);

	group = EC_KEY_get0_group(key);
	point = EC_POINT_new(group);
	if (!point)
	{
		EC_KEY_free(key);
		return (NULL);
	}

	/* 2. Convert the octet string back into an EC_POINT */
	if (!EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, NULL))
		goto error;

	/* 3. Set the public key in the EC_KEY structure */
	if (!EC_KEY_set_public_key(key, point))
		goto error;

	/* Cleanup the temporary point and return the key */
	EC_POINT_free(point);
	return (key);

error:
	EC_POINT_free(point);
	EC_KEY_free(key);
	return (NULL);
}
