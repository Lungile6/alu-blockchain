/* blockchain/v0.3/cli/main.c */
#include "cli.h"

/* Define globals */
EC_KEY *wallet = NULL;
blockchain_t *blockchain = NULL;
llist_t *transaction_pool = NULL;
llist_t *unspent_outputs = NULL;

int main(void)
{
	/* 1. Initialize Blockchain */
	blockchain = blockchain_deserialize("chain.bin");
	if (!blockchain)
		blockchain = blockchain_create();

	/* 2. Update header-declared function call */
	unspent_outputs = blockchain_get_unspent(blockchain);
	
	/* 3. Initialize Pool */
	transaction_pool = llist_create(MT_SUPPORT_FALSE);

	/* 4. Wallet Setup */
	wallet = ec_load("wallet");
	if (!wallet)
	{
		wallet = ec_create();
		ec_save(wallet, "wallet");
	}

	cli_loop();

	/* Cleanup */
	llist_destroy(transaction_pool, 1, (node_dtor_t)transaction_destroy);
	llist_destroy(unspent_outputs, 1, (node_dtor_t)free);
	blockchain_destroy(blockchain);
	EC_KEY_free(wallet);

	return (0);
}
