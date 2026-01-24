#include <stdlib.h>
#include "blockchain.h"

/**
 * block_destroy - Deallocates a Block structure
 * @block: Pointer to the Block to delete
 */
void block_destroy(block_t *block)
{
	if (!block)
		return;

	/* Destroy the transaction list and all transactions within it */
	llist_destroy(block->transactions, 1, (node_rel_t)transaction_destroy);

	free(block);
}
