#include <stdio.h>
#include <stdlib.h>
#include "blockchain.h"

/**
 * write_block - Writes a single block to the file
 * @node: Pointer to the block_t node
 * @idx: Index of the node (unused)
 * @arg: The file pointer (FILE *)
 *
 * Return: 0 on success, -1 on failure
 */
static int write_block(llist_node_t node, unsigned int idx, void *arg)
{
	block_t *block = (block_t *)node;
	FILE *fp = (FILE *)arg;

	(void)idx;

	if (!block || !fp)
		return (-1);

	/* Write Block Info */
	fwrite(&(block->info), sizeof(block->info), 1, fp);
	/* Write Block Data Length */
	fwrite(&(block->data.len), sizeof(block->data.len), 1, fp);
	/* Write Block Data Buffer */
	fwrite(block->data.buffer, block->data.len, 1, fp);
	/* Write Block Hash */
	fwrite(block->hash, SHA256_DIGEST_LENGTH, 1, fp);

	return (0);
}

/**
 * blockchain_serialize - Serializes a Blockchain into a file
 * @blockchain: Pointer to the Blockchain to serialize
 * @path: Path to the output file
 *
 * Return: 0 on success, -1 on failure
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *fp;
	uint8_t magic[] = "HBLK";
	uint8_t version[] = "0.1";
	uint8_t endian = 1; /* 1 for Little Endian */
	int32_t blocks_count;

	if (!blockchain || !path)
		return (-1);

	fp = fopen(path, "wb");
	if (!fp)
		return (-1);

	blocks_count = llist_size(blockchain->chain);

	/* Write Header: Magic, Version, Endian, Block Count */
	fwrite(magic, 4, 1, fp);
	fwrite(version, 3, 1, fp);
	fwrite(&endian, 1, 1, fp);
	fwrite(&blocks_count, 4, 1, fp);

	/* Use llist_for_each to iterate and write every block */
	if (llist_for_each(blockchain->chain, write_block, fp) == -1)
	{
		fclose(fp);
		return (-1);
	}

	fclose(fp);
	return (0);
}
