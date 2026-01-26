#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

/**
 * genesis_create - Initializes the Genesis Block with hardcoded values
 *
 * Return: A pointer to the initialized Genesis Block, or NULL on failure
 */
static block_t *genesis_create(void)
{
	block_t *genesis;

	genesis = malloc(sizeof(*genesis));
	if (!genesis)
		return (NULL);

	memset(genesis, 0, sizeof(*genesis));
	genesis->info.index = 0;
	genesis->info.difficulty = 0;
	genesis->info.timestamp = 1537578000;
	genesis->info.nonce = 0;

	memcpy(genesis->data.buffer, "Holberton School", 16);
	genesis->data.len = 16;

	memcpy(genesis->hash,
		"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"
		"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03",
		SHA256_DIGEST_LENGTH);

	return (genesis);
}

/**
 * blockchain_create - Creates a Blockchain structure and initializes it
 * with the Genesis Block.
 *
 * Return: A pointer to the newly created blockchain_t, or NULL on failure.
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain;
	block_t *genesis;

	blockchain = malloc(sizeof(*blockchain));
	if (!blockchain)
		return (NULL);

	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
	{
		free(blockchain);
		return (NULL);
	}

	genesis = genesis_create();
	if (!genesis)
	{
		llist_destroy(blockchain->chain, 0, NULL);
		free(blockchain);
		return (NULL);
	}

	if (llist_add_node(blockchain->chain, genesis, ADD_NODE_FRONT) != 0)
	{
		free(genesis);
		llist_destroy(blockchain->chain, 0, NULL);
		free(blockchain);
		return (NULL);
	}

	return (blockchain);
}
