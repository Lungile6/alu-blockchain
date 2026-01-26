#include <stdlib.h>
#include "blockchain.h"

/**
 * blockchain_destroy - Deletes a blockchain
 * @blockchain: Pointer to the blockchain to delete
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;

	llist_destroy(blockchain->chain, 1, (void (*)(void *))block_destroy);
	free(blockchain);
}
