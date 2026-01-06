#include "hblk_crypto.h"

/**
 * sha256 - Computes the hash of a sequence of bytes
 * @s: Sequence of bytes to be hashed
 * @len: Number of bytes to hash in s
 * @digest: Buffer to store the resulting hash
 *
 * Return: Pointer to digest, or NULL if digest is NULL
 */
uint8_t *sha256(int8_t const *s, size_t len, uint8_t digest[SHA256_DIGEST_LENGTH])
{
	if (!digest)
		return (NULL);

	return (SHA256((const unsigned char *)s, len, digest));
}
