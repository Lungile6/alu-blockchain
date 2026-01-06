#ifndef HBLK_CRYPTO_H
#define HBLK_CRYPTO_H

#include <stddef.h>
#include <stdint.h>
#include <openssl/sha.h>

/* OpenSSL Headers */
#include <openssl/sha.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>

/* --- Macros --- */
#define EC_CURVE NID_secp256k1

/* --- Prototypes --- */

/* SHA256_DIGEST_LENGTH is usually 32 bytes */

/**
 * sha256 - Computes the hash of a sequence of bytes
 * @s: Sequence of bytes to be hashed
 * @len: Number of bytes to hash in s
 * @digest: Buffer to store the resulting hash
 *
 * Return: Pointer to digest, or NULL if digest is NULL
 */
uint8_t *sha256(int8_t const *s, size_t len,
uint8_t digest[SHA256_DIGEST_LENGTH]);

/**
 * ec_create - Creates a new EC key pair
 *
 * Return: Pointer to EC_KEY structure, or NULL upon failure
 */
EC_KEY *ec_create(void);

#endif /* HBLK_CRYPTO_H */
