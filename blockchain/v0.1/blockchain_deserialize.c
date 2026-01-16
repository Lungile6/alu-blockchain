#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blockchain.h"

/**
 * read_block - Reads a single block from the file
 * @fp: File pointer to read from
 *
 * Return: Pointer to the allocated block, or NULL on failure
 */
static block_t *read_block(FILE *fp)
{
	block_t *block = malloc(sizeof(*block));

	if (!block)
		return (NULL);

	if (fread(&block->info, sizeof(block->info), 1, fp) != 1 ||
	    fread(&block->data.len, sizeof(block->data.len), 1, fp) != 1)
	{
		free(block);
		return (NULL);
	}

	memset(block->data.buffer, 0, BLOCKCHAIN_DATA_MAX);
	if (fread(block->data.buffer, 1, block->data.len, fp) != block->data.len ||
	    fread(block->hash, 1, SHA256_DIGEST_LENGTH, fp) != SHA256_DIGEST_LENGTH)
	{
		free(block);
		return (NULL);
	}

	return (block);
}

/**
 * blockchain_deserialize - Deserializes a Blockchain from a file
 * @path: Path to the file to load the Blockchain from
 *
 * Return: Pointer to the deserialized Blockchain, or NULL on failure
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *fp;
	blockchain_t *bc;
	uint8_t magic[4], version[3], endian;
	int32_t count, i;
	block_t *block;

	if (!path)
		return (NULL);

	fp = fopen(path, "rb");
	if (!fp)
		return (NULL);

	if (fread(magic, 1, 4, fp) != 4 || memcmp(magic, "HBLK", 4) != 0 ||
	    fread(version, 1, 3, fp) != 3 || memcmp(version, "0.1", 3) != 0 ||
	    fread(&endian, 1, 1, fp) != 1 || fread(&count, 4, 1, fp) != 1)
	{
		fclose(fp);
		return (NULL);
	}
	bc = malloc(sizeof(*bc));
	if (!bc)
	{
		fclose(fp);
		return (NULL);
	}
	bc->chain = llist_create(MT_SUPPORT_FALSE);
	for (i = 0; i < count; i++)
	{
		block = read_block(fp);
		if (!block)
		{
			blockchain_destroy(bc);
			fclose(fp);
			return (NULL);
		}
		llist_add_node(bc->chain, block, ADD_NODE_REAR);
	}
	fclose(fp);
	return (bc);
}
