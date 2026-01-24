#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * find_unspent - Matcher function to find a specific unspent output in a list
 * @node: Current node in the list (unspent_tx_out_t)
 * @arg: The hash to match against
 * Return: 1 if match found, 0 otherwise
 */
static int find_unspent(llist_node_t node, void *arg)
{
	unspent_tx_out_t *unspent = node;
	uint8_t *hash = arg;

	return (memcmp(unspent->out.hash, hash, SHA256_DIGEST_LENGTH) == 0);
}

/**
 * tx_in_sign - Signs a transaction input
 * @in: Points to the transaction input structure to sign
 * @tx_id: ID (hash) of the transaction the input is stored in
 * @sender: Private key of the receiver of the coins
 * @all_unspent: List of all unspent transaction outputs to date
 *
 * Return: Pointer to resulting signature structure, or NULL on failure
 */
sig_t *tx_in_sign(tx_in_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	EC_KEY const *sender, llist_t *all_unspent)
{
	uint8_t pub[EC_PUB_LEN];
	unspent_tx_out_t *utxo;

	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);

	/* 1. Find the referenced UTXO in the list */
	utxo = llist_find_node(all_unspent, find_unspent, (void *)in->tx_out_hash);
	if (!utxo)
	{
		fprintf(stderr, "tx_in_sign: Referenced UTXO not found\n");
		return (NULL);
	}

	/* 2. Verify that the sender owns this UTXO */
	if (!ec_to_pub(sender, pub) ||
		memcmp(pub, utxo->out.pub, EC_PUB_LEN) != 0)
	{
		fprintf(stderr, "tx_in_sign: Sender does not own this UTXO\n");
		return (NULL);
	}

	/* 3. Sign the transaction ID using the sender's private key */
	if (!ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig))
		return (NULL);

	return (&in->sig);
}
