#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * collect_utxos - Callback to select UTXOs and create inputs
 * @node: Current unspent_tx_out_t
 * @idx: Index of node (unused)
 * @arg: Pointer to utxo_search_t
 *
 * Return: 0 to continue, 1 if enough funds collected
 */
static int collect_utxos(llist_node_t node, unsigned int idx, void *arg)
{
	unspent_tx_out_t *utxo = node;
	utxo_search_t *search = arg;
	tx_in_t *in;

	(void)idx;

	if (memcmp(utxo->out.pub, search->sender_pub, EC_PUB_LEN) == 0)
	{
		in = tx_in_create(utxo);
		if (in)
		{
			llist_add_node(search->inputs, in, ADD_NODE_REAR);
			search->collected += utxo->out.amount;
		}

		if (search->collected >= search->amount)
			return (1);
	}
	return (0);
}

/**
 * sign_and_hash - Helper to hash and sign inputs to save lines
 * @tx: The transaction
 * @sender: Private key
 * @all_unspent: UTXO list
 *
 * Return: 1 on success, 0 on failure
 */
static int sign_and_hash(transaction_t *tx, EC_KEY const *sender,
	llist_t *all_unspent)
{
	int i, count;

	if (!transaction_hash(tx, tx->id))
		return (0);

	count = llist_size(tx->inputs);
	for (i = 0; i < count; i++)
	{
		tx_in_t *in = llist_get_node_at(tx->inputs, i);

		tx_in_sign(in, tx->id, sender, all_unspent);
	}
	return (1);
}

/**
 * transaction_create - Creates a transaction
 * @sender: Private key of the sender
 * @receiver: Public key of the receiver
 * @amount: Amount to send
 * @all_unspent: List of all unspent outputs to date
 *
 * Return: Pointer to created transaction, or NULL on failure
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
	uint32_t amount, llist_t *all_unspent)
{
	transaction_t *tx;
	utxo_search_t search;
	uint8_t pub[EC_PUB_LEN];

	if (!sender || !receiver || !all_unspent)
		return (NULL);
	tx = malloc(sizeof(*tx));
	if (!tx)
		return (NULL);
	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);
	ec_to_pub(sender, search.sender_pub);
	search.amount = amount, search.collected = 0, search.inputs = tx->inputs;
	llist_for_each(all_unspent, collect_utxos, &search);
	if (search.collected < amount)
	{
		llist_destroy(tx->inputs, 1, free), llist_destroy(tx->outputs, 1, free);
		return (free(tx), NULL);
	}
	ec_to_pub(receiver, pub);
	llist_add_node(tx->outputs, tx_out_create(amount, pub), ADD_NODE_REAR);
	if (search.collected > amount)
		llist_add_node(tx->outputs, tx_out_create(search.collected - amount,
			search.sender_pub), ADD_NODE_REAR);
	if (!sign_and_hash(tx, sender, all_unspent))
		return (NULL);
	return (tx);
}
