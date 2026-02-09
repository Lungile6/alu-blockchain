#include "blockchain.h"

/**
 * blockchain_get_unspent - Collects all unspent transaction outputs (UTXOs)
 * @blockchain: Pointer to the blockchain to analyze
 *
 * Return: A new linked list of unspent_tx_out_t pointers
 */
llist_t *blockchain_get_unspent(blockchain_t const *blockchain)
{
	llist_t *unspent;
	block_t *block;
	int i;

	if (!blockchain || !blockchain->chain)
		return (NULL);

	/* Create the initial empty list of UTXOs */
	unspent = llist_create(MT_SUPPORT_FALSE);
	if (!unspent)
		return (NULL);

	/*
	 * Iterate through every block in the chain.
	 * For each block, update the 'unspent' list using its transactions.
	 */
	for (i = 0; i < llist_size(blockchain->chain); i++)
	{
		block = llist_get_node_at(blockchain->chain, i);

		/* update_unspent: removes spent outputs and adds new ones */
		unspent = update_unspent(block->transactions,
					 block->info.prev_hash, unspent);
	}

	return (unspent);
}
