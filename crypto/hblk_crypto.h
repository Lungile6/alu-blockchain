#ifndef HBLK_CRYPTO_H
#define HBLK_CRYPTO_H
#define PUB_FILENAME "key_pub.pem"
#define PRI_FILENAME "key.pem"

#include <stddef.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <openssl/pem.h>

/* OpenSSL Headers */
#include <openssl/sha.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>

/* --- Macros --- */
#define EC_CURVE NID_secp256k1

/* 1 byte (0x04) + 32 bytes (X) + 32 bytes (Y) = 65 */
#define EC_PUB_LEN 65

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

/**
 * ec_to_pub - Extracts the public key from an EC_KEY structure
 * @key: Pointer to the EC_KEY structure
 * @pub: Address to store the extracted public key
 *
 * Return: Pointer to pub, or NULL upon failure
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);

/**
 * ec_from_pub - Creates an EC_KEY structure given a public key
 * @pub: Contains the public key to be converted (uncompressed)
 *
 * Return: Pointer to the created EC_KEY structure, or NULL upon failure
 */
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN]);

/**
 * ec_save - Saves an existing EC key pair on the disk
 * @key: Points to the EC key pair to be saved
 * @folder: Path to the folder in which to save the keys
 *
 * Return: 1 upon success, 0 upon failure
 */
int ec_save(EC_KEY *key, char const *folder);

/**
 * ec_load - Loads an EC key pair from the disk
 * @folder: Path to the folder from which to load the keys
 *
 * Return: Pointer to the created EC_KEY structure, or NULL upon failure
 */
EC_KEY *ec_load(char const *folder);

#define SIG_MAX_LEN 72

/**
 * struct sig_s - EC signature structure
 *
 * @sig: Signature buffer
 * @len: Actual signature length
 */
struct sig_s
{
	uint8_t sig[SIG_MAX_LEN];
	uint8_t len;
};

/**
 * sig_t - Typedef for struct sig_s
 */
typedef struct sig_s sig_t;

/**
 * ec_sign - Signs a given set of bytes, using a given EC_KEY private key
 * @key: Pointer to the EC_KEY structure containing the private key
 * @msg: Pointer to the characters to be signed
 * @msglen: Length of the message to sign
 * @sig: Address at which to store the signature
 *
 * Return: Pointer to the signature buffer (sig->sig) or NULL on failure
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg,
size_t msglen, sig_t *sig);

/**
 * ec_verify - Verifies the signature of a given set of bytes
 * @key: Pointer to the EC_KEY structure containing the public key
 * @msg: Pointer to the characters to verify the signature of
 * @msglen: Length of the message to verify
 * @sig: Pointer to the signature to be checked
 *
 * Return: 1 if the signature is valid, and 0 otherwise
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen, sig_t const *sig);

#endif /* HBLK_CRYPTO_H */
