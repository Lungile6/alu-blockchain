#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * coinbase_create - Creates a coinbase transaction
 * @receiver: Public key of the miner (receiver of the reward)
 * @block_index: Index of the block the coinbase transaction belongs to
 *
 * Return: Pointer to the created transaction, or NULL on failure
 */
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *tx;
	tx_in_t *in;
	uint8_t pub[EC_PUB_LEN];

	if (!receiver)
		return (NULL);

	tx = malloc(sizeof(*tx));
	if (!tx)
		return (NULL);

	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	in = malloc(sizeof(*in));
	if (!in)
	{
		llist_destroy(tx->inputs, 1, free);
		llist_destroy(tx->outputs, 1, free);
		return (free(tx), NULL);
	}

	memset(in, 0, sizeof(*in));
	memcpy(in->tx_out_hash, &block_index, sizeof(block_index));
	llist_add_node(tx->inputs, in, ADD_NODE_REAR);

	ec_to_pub(receiver, pub);
	llist_add_node(tx->outputs, tx_out_create(COINBASE_AMOUNT, pub), ADD_NODE_REAR);

	if (!transaction_hash(tx, tx->id))
		return (NULL);

	return (tx);
}
