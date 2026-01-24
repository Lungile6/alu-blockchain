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
	blockchain_t *blockchain;
	block_t *genesis;

	blockchain = malloc(sizeof(*blockchain));
	if (!blockchain)
		return (NULL);

	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	blockchain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain || !blockchain->unspent)
	{
		llist_destroy(blockchain->chain, 0, NULL);
		llist_destroy(blockchain->unspent, 0, NULL);
		free(blockchain);
		return (NULL);
	}

	genesis = malloc(sizeof(*genesis));
	if (!genesis)
	{
		llist_destroy(blockchain->chain, 0, NULL);
		llist_destroy(blockchain->unspent, 0, NULL);
		free(blockchain);
		return (NULL);
	}

	/* Initialize Genesis block (v0.3 requires the transaction list) */
	memset(genesis, 0, sizeof(*genesis));
	memcpy(genesis->hash, GENESIS_HASH, SHA256_DIGEST_LENGTH);
	genesis->info.timestamp = GENESIS_TIMESTAMP;
	memcpy(genesis->data.buffer, GENESIS_DATA, GENESIS_DATA_LEN);
	genesis->data.len = GENESIS_DATA_LEN;
	genesis->transactions = llist_create(MT_SUPPORT_FALSE);

	if (llist_add_node(blockchain->chain, genesis, ADD_NODE_FRONT) != 0)
		return (NULL);

	return (blockchain);
}
