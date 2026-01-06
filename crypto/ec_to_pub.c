#include "hblk_crypto.h"

/**
 * ec_to_pub - Extracts the public key from an EC_KEY opaque structure
 * @key: Pointer to the EC_KEY structure to retrieve the public key from
 * @pub: Address at which to store the extracted public key
 *
 * Return: Pointer to pub, or NULL upon failure
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_GROUP *group;
	const EC_POINT *point;

	if (!key || !pub)
		return (NULL);

	/* 1. Get the group (the curve parameters) from the key */
	group = EC_KEY_get0_group(key);
	/* 2. Get the public point from the key */
	point = EC_KEY_get0_public_key(key);

	if (!group || !point)
		return (NULL);

	/* 3. Convert the point to octet string (uncompressed format) */
	if (!EC_POINT_point2oct(group, point, POINT_CONVERSION_UNCOMPRESSED,
				pub, EC_PUB_LEN, NULL))
		return (NULL);

	return (pub);
}
