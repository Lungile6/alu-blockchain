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

	/* Only destroy the list if it actually exists (Genesis might be NULL) */
	if (block->transactions)
	{
		llist_destroy(block->transactions, 1,
			(void (*)(void *))transaction_destroy);
	}

	free(block);
}
