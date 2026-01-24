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
	/* We cast transaction_destroy to the generic destructor type */
	llist_destroy(block->transactions, 1, (void (*)(void *))transaction_destroy);

	free(block);
}
