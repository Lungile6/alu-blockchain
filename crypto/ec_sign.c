#include "hblk_crypto.h"

/**
 * ec_sign - Signs a given set of bytes using an EC_KEY private key
 * @key: Pointer to the EC_KEY structure containing the private key
 * @msg: Pointer to the characters to be signed
 * @msglen: Length of the message to sign
 * @sig: Pointer to the structure to store the signature
 *
 * Return: Pointer to the signature buffer (sig->sig) or NULL on failure
 */
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg,
		size_t msglen, sig_t *sig)
{
	unsigned int len;

	if (!key || !msg || !sig)
		return (NULL);

	/*
	 * ECDSA_sign performs the signing operation.
	 * 0 is used for the 'type' parameter (reserved for future use).
	 * msg: the data to sign
	 * msglen: length of that data
	 * sig->sig: buffer where the signature will be written
	 * &len: pointer to an unsigned int where the actual sig size is stored
	 * key: the EC_KEY containing the private key
	 */
	if (ECDSA_sign(0, msg, (int)msglen, sig->sig, &len, (EC_KEY *)key) != 1)
	{
		return (NULL);
	}

	/* Update the sig_t structure with the resulting length */
	sig->len = (uint8_t)len;

	return (sig->sig);
}
