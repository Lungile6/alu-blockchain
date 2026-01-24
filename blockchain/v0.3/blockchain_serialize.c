#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "blockchain.h"

/**
 * serialize_tx - Helper to serialize transactions within a block
 * @fd: File descriptor
 * @transactions: List of transactions
 * Return: 0 on success, -1 on failure
 */
static int serialize_tx(int fd, llist_t *transactions)
{
	int i, j, nb_tx, nb_in, nb_out;
	transaction_t *tx;
	tx_in_t *in;
	tx_out_t *out;

	nb_tx = llist_size(transactions);
	write(fd, &nb_tx, 4);
	for (i = 0; i < nb_tx; i++)
	{
		tx = llist_get_node_at(transactions, i);
		write(fd, tx->id, 32);
		nb_in = llist_size(tx->inputs);
		nb_out = llist_size(tx->outputs);
		write(fd, &nb_in, 4);
		write(fd, &nb_out, 4);
		for (j = 0; j < nb_in; j++)
		{
			in = llist_get_node_at(tx->inputs, j);
			write(fd, in, 169);
		}
		for (j = 0; j < nb_out; j++)
		{
			out = llist_get_node_at(tx->outputs, j);
			write(fd, out, 101);
		}
	}
	return (0);
}

/**
 * blockchain_serialize - Serializes a Blockchain into a file
 * @blockchain: Pointer to the Blockchain to serialize
 * @path: Path to the file
 * Return: 0 on success, -1 on failure
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	int fd, i, nb_blocks, nb_unspent;
	block_t *block;
	unspent_tx_out_t *unspent;
	uint8_t endian = _get_endianness();

	if (!blockchain || !path)
		return (-1);
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	nb_blocks = llist_size(blockchain->chain);
	nb_unspent = llist_size(blockchain->unspent);
	write(fd, "HBLK0.3", 7);
	write(fd, &endian, 1);
	write(fd, &nb_blocks, 4);
	write(fd, &nb_unspent, 4);
	for (i = 0; i < nb_blocks; i++)
	{
		block = llist_get_node_at(blockchain->chain, i);
		write(fd, &block->info, 56);
		write(fd, &block->data.len, 4);
		write(fd, block->data.buffer, block->data.len);
		write(fd, block->hash, 32);
		serialize_tx(fd, block->transactions);
	}
	for (i = 0; i < nb_unspent; i++)
	{
		unspent = llist_get_node_at(blockchain->unspent, i);
		write(fd, unspent, 165);
	}
	return (close(fd) == -1 ? -1 : 0);
}
