#include "hblk_crypto.h"

/**
 * ec_verify - Verifies the signature of a given set of bytes
 * @key: Pointer to the EC_KEY structure containing the public key
 * @msg: Pointer to the characters to verify the signature of
 * @msglen: Length of the message to verify
 * @sig: Pointer to the signature to be checked
 *
 * Return: 1 if the signature is valid, and 0 otherwise
 */
int ec_verify(EC_KEY const *key, uint8_t const *msg,
		size_t msglen, sig_t const *sig)
{
	if (!key || !msg || !sig)
		return (0);

	/*
	 * ECDSA_verify takes:
	 * 0: type (ignored)
	 * msg: the original data
	 * msglen: length of data
	 * sig->sig: the signature buffer
	 * sig->len: the length of the signature
	 * key: the EC_KEY containing the public key
	 * Returns: 1 on success, 0 on invalid signature, -1 on error
	 */
	if (ECDSA_verify(0, msg, (int)msglen, sig->sig, sig->len,
				(EC_KEY *)key) != 1)
	{
		return (0);
	}

	return (1);
}
