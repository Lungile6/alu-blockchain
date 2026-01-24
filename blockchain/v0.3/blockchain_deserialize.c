#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

/**
 * deserialize_tx - Helper to read transactions from file
 * @fd: File descriptor
 * @nb_tx: Number of transactions to read
 * Return: Pointer to llist of transactions
 */
static llist_t *deserialize_tx(int fd, int nb_tx)
{
	llist_t *list = llist_create(MT_SUPPORT_FALSE);
	int i, j, nb_in, nb_out;
	transaction_t *tx;
	tx_in_t *in;
	tx_out_t *out;

	for (i = 0; i < nb_tx; i++)
	{
		tx = calloc(1, sizeof(*tx));
		read(fd, tx->id, 32);
		read(fd, &nb_in, 4);
		read(fd, &nb_out, 4);
		tx->inputs = llist_create(MT_SUPPORT_FALSE);
		tx->outputs = llist_create(MT_SUPPORT_FALSE);
		for (j = 0; j < nb_in; j++)
		{
			in = calloc(1, sizeof(*in));
			read(fd, in, 169);
			llist_add_node(tx->inputs, in, ADD_NODE_REAR);
		}
		for (j = 0; j < nb_out; j++)
		{
			out = calloc(1, sizeof(*out));
			read(fd, out, 101);
			llist_add_node(tx->outputs, out, ADD_NODE_REAR);
		}
		llist_add_node(list, tx, ADD_NODE_REAR);
	}
	return (list);
}

/**
 * blockchain_deserialize - Deserializes a Blockchain from a file
 * @path: Path to the file
 * Return: Pointer to the Blockchain, or NULL
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	int fd, i, blocks, unspent, nb_tx;
	blockchain_t *bc;
	block_t *block;
	unspent_tx_out_t *utxo;
	char magic[8];

	fd = open(path, O_RDONLY);
	if (fd == -1 || read(fd, magic, 7) != 7 || memcmp(magic, "HBLK0.3", 7))
		return (fd == -1 ? NULL : (close(fd), NULL));
	lseek(fd, 8, SEEK_SET);
	read(fd, &blocks, 4);
	read(fd, &unspent, 4);
	bc = malloc(sizeof(*bc));
	bc->chain = llist_create(MT_SUPPORT_FALSE);
	bc->unspent = llist_create(MT_SUPPORT_FALSE);
	for (i = 0; i < blocks; i++)
	{
		block = calloc(1, sizeof(*block));
		read(fd, &block->info, 56);
		read(fd, &block->data.len, 4);
		read(fd, block->data.buffer, block->data.len);
		read(fd, block->hash, 32);
		read(fd, &nb_tx, 4);
		block->transactions = (nb_tx == -1) ? NULL : deserialize_tx(fd, nb_tx);
		llist_add_node(bc->chain, block, ADD_NODE_REAR);
	}
	for (i = 0; i < unspent; i++)
	{
		utxo = calloc(1, sizeof(*utxo));
		read(fd, utxo, 165);
		llist_add_node(bc->unspent, utxo, ADD_NODE_REAR);
	}
	return (close(fd), bc);
}
