#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * utxo_search_s - Helper structure for UTXO collection
 */
typedef struct utxo_search_s {
	uint8_t sender_pub[EC_PUB_LEN];
	uint32_t amount;
	uint32_t collected;
	llist_t *inputs;
} utxo_search_t;

/**
 * collect_utxos - Iterates through all_unspent to find coins owned by sender
 * @node: Current unspent_tx_out_t node
 * @idx: Index (unused)
 * @arg: Pointer to utxo_search_t
 * Return: 0 to continue, 1 to stop
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
 * transaction_create - Creates a transaction
 * @sender: Private key of the sender
 * @receiver: Public key of the receiver
 * @amount: Amount to send
 * @all_unspent: List of all UTXOs
 *
 * Return: Pointer to created transaction, or NULL on failure
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver,
	uint32_t amount, llist_t *all_unspent)
{
	transaction_t *tx;
	utxo_search_t search;
	tx_out_t *out;
	uint8_t receiver_pub[EC_PUB_LEN];
	int i, inputs_count; /* Declarations at the top for C90 */

	if (!sender || !receiver || !all_unspent)
		return (NULL);

	tx = malloc(sizeof(*tx));
	if (!tx) return (NULL);

	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);

	ec_to_pub(sender, search.sender_pub);
	search.amount = amount;
	search.collected = 0;
	search.inputs = tx->inputs;

	llist_for_each(all_unspent, collect_utxos, &search);

	if (search.collected < amount)
	{
		llist_destroy(tx->inputs, 1, free);
		llist_destroy(tx->outputs, 1, free);
		free(tx);
		return (NULL);
	}

	ec_to_pub(receiver, receiver_pub);
	out = tx_out_create(amount, receiver_pub);
	llist_add_node(tx->outputs, out, ADD_NODE_REAR);

	if (search.collected > amount)
	{
		out = tx_out_create(search.collected - amount, search.sender_pub);
		llist_add_node(tx->outputs, out, ADD_NODE_REAR);
	}

	if (!transaction_hash(tx, tx->id))
		return (NULL);

	/* Use llist_get_node_at for safer/standard iteration in C90 */
	inputs_count = llist_size(tx->inputs);
	for (i = 0; i < inputs_count; i++)
	{
		tx_in_t *in = llist_get_node_at(tx->inputs, i);
		tx_in_sign(in, tx->id, sender, all_unspent);
	}

	return (tx);
}
