#include <stdlib.h>
#include "blockchain.h"

/**
 * blockchain_destroy - Deallocates a Blockchain structure
 * @blockchain: Pointer to the Blockchain to delete
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;

	/* Destroy chain and blocks (using the cast to avoid node_rel_t error) */
	llist_destroy(blockchain->chain, 1, (void (*)(void *))block_destroy);

	/* Destroy unspent output list */
	llist_destroy(blockchain->unspent, 1, free);

	free(blockchain);
}
