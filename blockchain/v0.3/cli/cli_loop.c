#include "cli.h"

/**
 * _hex_to_bin - Converts a hex string to a binary buffer (C90 compliant)
 * @hex: The hex string
 * @bin: The buffer to store binary data
 * @len: Length of the binary buffer
 * Return: 0 on success, -1 on failure
 */
static int _hex_to_bin(char const *hex, uint8_t *bin, size_t len)
{
	size_t i;
	unsigned int val;

	if (!hex || strlen(hex) != len * 2)
		return (-1);

	for (i = 0; i < len; i++)
	{
		/* Read into unsigned int first to satisfy C90/pedantic */
		if (sscanf(hex + (i * 2), "%02x", &val) != 1)
			return (-1);
		bin[i] = (uint8_t)val;
	}
	return (0);
}

/**
 * cmd_send - Creates a transaction and adds it to the local pool
 * @amount_str: String representation of the amount
 * @address_str: Hex string of the receiver's public key
 * Return: 0 on success, 1 on failure
 */
int cmd_send(char const *amount_str, char const *address_str)
{
	uint32_t amount;
	transaction_t *tx;
	EC_KEY *receiver_pub_key;
	uint8_t pub_bin[EC_PUB_LEN];

	if (!amount_str || !address_str)
	{
		fprintf(stderr, "Usage: send <amount> <address>\n");
		return (1);
	}
	amount = (uint32_t)atoi(amount_str);
	if (_hex_to_bin(address_str, pub_bin, EC_PUB_LEN) == -1)
	{
		fprintf(stderr, "Error: Invalid hex address format\n");
		return (1);
	}
	receiver_pub_key = ec_from_pub(pub_bin);
	if (!receiver_pub_key)
	{
		fprintf(stderr, "Error: Invalid receiver address\n");
		return (1);
	}
	tx = transaction_create(wallet, receiver_pub_key, amount, unspent_outputs);
	EC_KEY_free(receiver_pub_key);
	if (!tx)
	{
		fprintf(stderr, "Error: Failed to create transaction\n");
		return (1);
	}
	if (transaction_is_valid(tx, unspent_outputs) != 1)
	{
		fprintf(stderr, "Error: Created transaction is invalid\n");
		transaction_destroy(tx);
		return (1);
	}
	llist_add_node(transaction_pool, tx, ADD_NODE_REAR);
	printf("Transaction successfully created and added to the pool!\n");
	return (0);
}

/**
 * cmd_wallet_load - Loads a wallet from a given path
 * @path: Path to the wallet directory
 * Return: 0 on success, 1 on failure
 */
int cmd_wallet_load(char const *path)
{
	EC_KEY *tmp;

	if (!path)
	{
		fprintf(stderr, "Usage: wallet_load <path>\n");
		return (1);
	}
	tmp = ec_load(path);
	if (!tmp)
	{
		fprintf(stderr, "Failed to load wallet from %s\n", path);
		return (1);
	}
	if (wallet)
		EC_KEY_free(wallet);
	wallet = tmp;
	printf("Wallet successfully loaded from %s\n", path);
	return (0);
}

/**
 * cmd_wallet_save - Saves the current wallet to a given path
 * @path: Path to the directory where keys will be saved
 * Return: 0 on success, 1 on failure
 */
int cmd_wallet_save(char const *path)
{
	if (!path)
	{
		fprintf(stderr, "Usage: wallet_save <path>\n");
		return (1);
	}
	if (!ec_save(wallet, path))
	{
		fprintf(stderr, "Failed to save wallet to %s\n", path);
		return (1);
	}
	printf("Wallet successfully saved to %s\n", path);
	return (0);
}

/**
 * cmd_mine - Mines a new block with transactions from the pool
 *
 * Return: 0 on success, 1 on failure
 */
int cmd_mine(void)
{
	block_t *latest, *new_block;
	transaction_t *tx, *coinbase;
	uint32_t difficulty;
	int i;

	latest = llist_get_tail(blockchain->chain);
	difficulty = blockchain_difficulty(blockchain);
	new_block = block_create(latest, (int8_t *)"Holberton", difficulty);

	/* 1. Inject Coinbase Transaction (First position) */
	coinbase = coinbase_create(wallet, latest->info.index + 1);
	if (!coinbase || !coinbase_is_valid(coinbase, latest->info.index + 1))
	{
		fprintf(stderr, "Error: Failed to create coinbase\n");
		block_destroy(new_block);
		return (1);
	}
	llist_add_node(new_block->transactions, coinbase, ADD_NODE_REAR);
	unspent_outputs = update_unspent(new_block->transactions,
					 latest->hash, unspent_outputs);

	/* 2. Process Pool Transactions 1-by-1 */
	for (i = 0; i < llist_size(transaction_pool); i++)
	{
		tx = llist_get_node_at(transaction_pool, i);
		if (transaction_is_valid(tx, unspent_outputs))
		{
			llist_t *tmp_list = llist_create(MT_SUPPORT_FALSE);

			llist_add_node(new_block->transactions, tx, ADD_NODE_REAR);
			/* Update UTXO set immediately after processing */
			llist_add_node(tmp_list, tx, ADD_NODE_REAR);
			unspent_outputs = update_unspent(tmp_list,
							 latest->hash, unspent_outputs);
			llist_destroy(tmp_list, 0, NULL);
		}
		else
		{
			transaction_destroy(tx);
		}
	}
	/* Empty the pool */
	llist_destroy(transaction_pool, 0, NULL);
	transaction_pool = llist_create(MT_SUPPORT_FALSE);

	/* 3. Mine and Validate Block */
	block_mine(new_block);

	if (block_is_valid(new_block, latest, unspent_outputs) != 0)
	{
		fprintf(stderr, "Error: Mined block is invalid\n");
		block_destroy(new_block);
		return (1);
	}

	llist_add_node(blockchain->chain, new_block, ADD_NODE_REAR);
	printf("Block mined successfully! (Index: %u)\n", new_block->info.index);
	return (0);
}

/**
 * cli_loop - Main command loop
 */
void cli_loop(void)
{
	char *line = NULL, *cmd, *arg1, *arg2;
	size_t len = 0;
	ssize_t nread;

	while (1)
	{
		printf("blockchain> ");
		nread = getline(&line, &len, stdin);
		if (nread == -1)
			break;
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';
		cmd = strtok(line, " \t");
		if (!cmd)
			continue;
		arg1 = strtok(NULL, " \t");
		arg2 = strtok(NULL, " \t");
		if (strcmp(cmd, "exit") == 0)
			break;
		else if (strcmp(cmd, "wallet_load") == 0)
			cmd_wallet_load(arg1);
		else if (strcmp(cmd, "wallet_save") == 0)
			cmd_wallet_save(arg1);
		else if (strcmp(cmd, "send") == 0)
			cmd_send(arg1, arg2);
		else if (strcmp(cmd, "mine") == 0)
			cmd_mine();
		else if (strcmp(cmd, "info") == 0)
			cmd_info();
		else if (strcmp(cmd, "load") == 0)
			cmd_load(arg1);
		else if (strcmp(cmd, "save") == 0)
			cmd_save(arg1);
		else
			printf("Unknown command: %s\n", cmd);
	}
	free(line);
}

/**
 * cmd_info - Displays the state of the blockchain and wallet
 * Return: 0
 */
int cmd_info(void)
{
	uint8_t pub[EC_PUB_LEN];
	size_t i, balance = 0;
	unspent_tx_out_t *utxo;

	printf("--- Blockchain Summary ---\n");
	printf("Chain height: %d blocks\n", llist_size(blockchain->chain));
	printf("Pending Txs:  %d\n", llist_size(transaction_pool));
	printf("Total UTXOs:  %d\n", llist_size(unspent_outputs));

	if (wallet && ec_to_pub(wallet, pub))
	{
		for (i = 0; i < (size_t)llist_size(unspent_outputs); i++)
		{
			utxo = llist_get_node_at(unspent_outputs, i);
			if (memcmp(utxo->out.pub, pub, EC_PUB_LEN) == 0)
				balance += utxo->out.amount;
		}
		printf("Wallet Balance: %lu coins\n", balance);
	}
	printf("--------------------------\n");
	return (0);
}

/**
 * cmd_load - Loads a blockchain from a file
 * @path: Path to the serialized blockchain file
 *
 * Return: 0 on success, 1 on failure
 */
int cmd_load(char const *path)
{
	blockchain_t *new_chain;
	llist_t *new_unspent;

	if (!path)
	{
		fprintf(stderr, "Usage: load <path>\n");
		return (1);
	}

	/* 1. Attempt to deserialize the file */
	new_chain = blockchain_deserialize(path);
	if (!new_chain)
	{
		fprintf(stderr, "Error: Failed to load blockchain from %s\n", path);
		return (1);
	}

	/* 2. Get the UTXO state for the new chain */
	new_unspent = blockchain_get_unspent(new_chain);
	if (!new_unspent)
	{
		fprintf(stderr, "Error: Failed to compute UTXO set for new chain\n");
		blockchain_destroy(new_chain);
		return (1);
	}

	/* 3. Success: Cleanup the old chain/UTXOs and replace them */
	blockchain_destroy(blockchain);
	llist_destroy(unspent_outputs, 1, (node_dtor_t)free);

	blockchain = new_chain;
	unspent_outputs = new_unspent;

	printf("Blockchain successfully loaded from %s\n", path);
	return (0);
}

/**
 * cmd_save - Saves the current blockchain to a file
 * @path: Path to the destination file
 *
 * Return: 0 on success, 1 on failure
 */
int cmd_save(char const *path)
{
	if (!path)
	{
		fprintf(stderr, "Usage: save <path>\n");
		return (1);
	}

	/* blockchain_serialize returns 0 on success, -1 on failure */
	if (blockchain_serialize(blockchain, path) != 0)
	{
		fprintf(stderr, "Error: Failed to save blockchain to %s\n", path);
		return (1);
	}

	printf("Blockchain successfully saved to %s\n", path);
	return (0);
}

/**
 * cmd_exit - Clean up and exit
 */
void cmd_exit(void)
{
	if (wallet)
		EC_KEY_free(wallet);
	if (blockchain)
		blockchain_destroy(blockchain);
	printf("Exiting...\n");
}
