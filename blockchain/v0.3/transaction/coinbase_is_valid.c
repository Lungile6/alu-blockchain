#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * coinbase_is_valid - Verifies if a coinbase transaction is valid
 * @coinbase: Points to the coinbase transaction to verify
 * @block_index: Index of the Block the coinbase belongs to
 *
 * Return: 1 if valid, 0 otherwise
 */
int coinbase_is_valid(transaction_t const *coinbase, uint32_t block_index)
{
	uint8_t check_id[SHA256_DIGEST_LENGTH], zero[sizeof(sig_t)] = {0};
	tx_in_t *in;
	tx_out_t *out;

	if (!coinbase)
		return (0);

	/* 1. Verify Hash ID */
	if (!transaction_hash(coinbase, check_id) ||
		memcmp(check_id, coinbase->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	/* 2. Verify List Sizes (Exactly 1 input and 1 output) */
	if (llist_size(coinbase->inputs) != 1 || llist_size(coinbase->outputs) != 1)
		return (0);

	/* 3. Verify Input Content */
	in = llist_get_node_at(coinbase->inputs, 0);
	if (memcmp(in->tx_out_hash, &block_index, sizeof(block_index)) != 0 ||
		memcmp(in->tx_out_hash + sizeof(block_index), zero,
			SHA256_DIGEST_LENGTH - sizeof(block_index)) != 0 ||
		memcmp(in->block_hash, zero, SHA256_DIGEST_LENGTH) != 0 ||
		memcmp(in->tx_id, zero, SHA256_DIGEST_LENGTH) != 0 ||
		memcmp(&in->sig, zero, sizeof(sig_t)) != 0)
		return (0);

	/* 4. Verify Output Amount */
	out = llist_get_node_at(coinbase->outputs, 0);
	if (!out || out->amount != COINBASE_AMOUNT)
		return (0);

	return (1);
}
