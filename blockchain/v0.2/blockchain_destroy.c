#include <stdlib.h>
#include "blockchain.h"

/**
 * blockchain_destroy - Deletes an existing Blockchain and all its blocks
 *
 * @blockchain: Pointer to the Blockchain structure to delete
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;

	/*
	 * llist_destroy params:
	 * 1. The list to destroy
	 * 2. Flag (1) to destroy nodes (data inside nodes)
	 * 3. Destructor function for the data (our block_destroy)
	 */
	llist_destroy(blockchain->chain, 1, (node_dtor_t)block_destroy);

	free(blockchain);
}
