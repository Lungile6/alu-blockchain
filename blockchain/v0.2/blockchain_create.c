#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

/**
 * blockchain_create - Initializes a new blockchain structure
 *
 * Return: Pointer to the newly allocated blockchain, or NULL on failure
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain;
	block_t *genesis_copy;

	blockchain = malloc(sizeof(*blockchain));
	if (!blockchain)
		return (NULL);

	/* * Fix: Using MT_SUPPORT_FALSE because MT_DIRECT 
	 * is not defined in your llist.h 
	 */
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
	{
		free(blockchain);
		return (NULL);
	}

	genesis_copy = malloc(sizeof(*genesis_copy));
	if (!genesis_copy)
	{
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	/* Copy the global _genesis block provided by the project */
	memcpy(genesis_copy, &_genesis, sizeof(_genesis));

	/* Add genesis block to the beginning of the chain (ADD_NODE_FRONT) */
	if (llist_add_node(blockchain->chain, genesis_copy, ADD_NODE_FRONT) == -1)
	{
		free(genesis_copy);
		llist_destroy(blockchain->chain, 1, NULL);
		free(blockchain);
		return (NULL);
	}

	return (blockchain);
}
