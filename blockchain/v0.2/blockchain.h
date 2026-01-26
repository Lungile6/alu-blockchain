#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_

#include <stdint.h>
#include <stddef.h>

#include "../../llist/llist.h"
#define BLOCKCHAIN_DATA_MAX 1024

#include "../../crypto/hblk_crypto.h"
#include "provided/endianness.h"

#define GENESIS_DATA "Holberton School"
#define GENESIS_DATA_LEN 16
#define GENESIS_TIME 1537578000
#define GENESIS_HASH "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"

#define BLOCK_GENERATION_INTERVAL 1
#define DIFFICULTY_ADJUSTMENT_INTERVAL 8

/**
 * struct block_info_s - Block info structure
 */
typedef struct block_info_s
{
	uint32_t index;
	uint32_t difficulty;
	uint64_t timestamp;
	uint64_t nonce;
	uint8_t prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_data_s - Block data structure
 */
typedef struct block_data_s
{
	int8_t buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t len;
} block_data_t;

/**
 * struct block_s - Block structure
 */
typedef struct block_s
{
	block_info_t info;
	block_data_t data;
	uint8_t hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_s - Blockchain structure
 */
typedef struct blockchain_s
{
	llist_t *chain;
} blockchain_t;

/* Prototypes */
blockchain_t *blockchain_create(void);
void blockchain_destroy(blockchain_t *blockchain);
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len);
void block_destroy(block_t *block);
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int block_is_valid(block_t const *block, block_t const *prev_block);
/* Provided by the project */
block_t _genesis_block(void);

/* v0.2 specific */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH], uint32_t difficulty);
void block_mine(block_t *block);
uint32_t blockchain_difficulty(blockchain_t const *blockchain);

/* Serialization */
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
blockchain_t *blockchain_deserialize(char const *path);

/* Provided */
uint8_t _get_endianness(void);

#endif
