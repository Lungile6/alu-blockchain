#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * find_utxo - Matcher to find a UTXO by its output hash
 * @node: Current unspent_tx_out_t node
 * @arg: Hash to find
 * Return: 1 if match, 0 otherwise
 */
static int find_utxo(llist_node_t node, void *arg)
{
	unspent_tx_out_t *utxo = node;

	return (memcmp(utxo->out.hash, arg, SHA256_DIGEST_LENGTH) == 0);
}

/**
 * validate_inputs - Checks UTXO existence, signatures, and sums amounts
 * @tx: Transaction being verified
 * @all_unspent: List of UTXOs
 * @total_in: Pointer to store sum of input amounts
 * Return: 1 if all inputs valid, 0 otherwise
 */
static int validate_inputs(transaction_t const *tx, llist_t *all_unspent,
	uint32_t *total_in)
{
	tx_in_t *in;
	unspent_tx_out_t *utxo;
	int i, count;
	EC_KEY *key;

	count = llist_size(tx->inputs);
	for (i = 0; i < count; i++)
	{
		in = llist_get_node_at(tx->inputs, i);
		utxo = llist_find_node(all_unspent, find_utxo, in->tx_out_hash);
		if (!utxo)
			return (0);

		key = ec_from_pub(utxo->out.pub);
		if (!key)
			return (0);
		if (!ec_verify(key, tx->id, SHA256_DIGEST_LENGTH, &in->sig))
		{
			EC_KEY_free(key);
			return (0);
		}
		EC_KEY_free(key);
		*total_in += utxo->out.amount;
	}
	return (1);
}

/**
 * transaction_is_valid - Verifies a transaction
 * @transaction: Transaction to verify
 * @all_unspent: List of all unspent outputs
 *
 * Return: 1 if valid, 0 otherwise
 */
int transaction_is_valid(transaction_t const
		*transaction, llist_t *all_unspent)
{
	uint8_t check_id[SHA256_DIGEST_LENGTH];
	uint32_t total_in = 0, total_out = 0;
	tx_out_t *out;
	int i, count;

	if (!transaction || !all_unspent)
		return (0);

	/* 1. Verify Transaction ID (Hash) */
	if (!transaction_hash(transaction, check_id) ||
		memcmp(check_id, transaction->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	/* 2. Verify Inputs (UTXO existence, signatures, amount) */
	if (!validate_inputs(transaction, all_unspent, &total_in))
		return (0);

	/* 3. Sum Outputs */
	count = llist_size(transaction->outputs);
	for (i = 0; i < count; i++)
	{
		out = llist_get_node_at(transaction->outputs, i);
		total_out += out->amount;
	}

	/* 4. Match Input Amount to Output Amount */
	return (total_in == total_out);
}
