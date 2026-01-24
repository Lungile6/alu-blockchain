#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

/**
 * blockchain_create - Creates a new Blockchain structure
 *
 * Return: Pointer to the created Blockchain, or NULL on failure
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *bc;
	block_t *gen;

	bc = malloc(sizeof(*bc));
	if (!bc)
		return (NULL);

	bc->chain = llist_create(MT_SUPPORT_FALSE);
	bc->unspent = llist_create(MT_SUPPORT_FALSE);
	gen = malloc(sizeof(*gen));
	if (!bc->chain || !bc->unspent || !gen)
	{
		llist_destroy(bc->chain, 0, NULL), llist_destroy(bc->unspent, 0, NULL);
		return (free(gen), free(bc), NULL);
	}

	memset(gen, 0, sizeof(*gen));
	memcpy(gen->hash, GENESIS_HASH, SHA256_DIGEST_LENGTH);
	gen->info.timestamp = GENESIS_TIME;
	memcpy(gen->data.buffer, GENESIS_DATA, GENESIS_DATA_LEN);
	gen->data.len = GENESIS_DATA_LEN;

	gen->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!gen->transactions || llist_add_node(bc->chain, gen, ADD_NODE_FRONT))
	{
		llist_destroy(bc->chain, 0, NULL), llist_destroy(bc->unspent, 0, NULL);
		return (llist_destroy(gen->transactions, 0, NULL),
				free(gen), free(bc), NULL);
	}

	return (bc);
}
