#include <stdlib.h>
#include <string.h>
#include "transaction.h"

/**
 * find_spent - Predicate to find and remove spent outputs
 * @node: Current unspent output in the list
 * @arg: The transaction input being checked
 * Return: 1 if match found (spent), 0 otherwise
 */
static int find_spent(unspent_tx_out_t *node, tx_in_t *arg)
{
	return (!memcmp(node->block_hash, arg->block_hash, SHA256_DIGEST_LENGTH) &&
			!memcmp(node->tx_id, arg->tx_id, SHA256_DIGEST_LENGTH) &&
			!memcmp(node->out.hash, arg->tx_out_hash, SHA256_DIGEST_LENGTH));
}

/**
 * update_unspent - Updates the list of unspent transaction outputs
 * @transactions: List of validated transactions in a block
 * @b_hash: Hash of the block containing the transactions
 * @all_unspent: Current list of unspent transaction outputs
 * Return: Pointer to the new list of unspent transaction outputs
 */
llist_t *update_unspent(llist_t *transactions,
	uint8_t b_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	int i, j, k;
	transaction_t *tx;
	tx_in_t *in;
	tx_out_t *out;

	for (i = 0; i < llist_size(transactions); i++)
	{
		tx = llist_get_node_at(transactions, i);
		for (j = 0; j < llist_size(tx->inputs); j++)
		{
			in = llist_get_node_at(tx->inputs, j);
			llist_remove_node(all_unspent, (node_ident_t)find_spent, in, 1, free);
		}
		for (k = 0; k < llist_size(tx->outputs); k++)
		{
			out = llist_get_node_at(tx->outputs, k);
			llist_add_node(all_unspent,
				unspent_tx_out_create(b_hash, tx->id, out), ADD_NODE_REAR);
		}
	}
	return (all_unspent);
}
