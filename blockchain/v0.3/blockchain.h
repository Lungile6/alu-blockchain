#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <time.h>
#include <llist.h>

/* * 1. Include the transaction header. 
 * Use the relative path to ensure the compiler finds it.
 */
#include "transaction/transaction.h"
#include "../../crypto/hblk_crypto.h"

/* Constants */
#define GENESIS_TIMESTAMP 1537578000
#define GENESIS_DATA "Holberton School"
#define GENESIS_HASH "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"

/**
 * struct block_info_s - Block info structure
 */
typedef struct block_info_s
{
	uint32_t    index;
	uint32_t    difficulty;
	uint64_t    timestamp;
	uint64_t    nonce;
	uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_s - Block structure
 */
typedef struct block_s
{
	block_info_t    info;
	llist_t         *transactions;
	uint8_t         hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_s - Blockchain structure
 */
typedef struct blockchain_s
{
	llist_t     *chain;
	llist_t     *unspent;
} blockchain_t;

/* --- v0.1 & v0.2 Functions --- */
blockchain_t *blockchain_create(void);
void blockchain_destroy(blockchain_t *blockchain);
void block_destroy(block_t *block);
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int block_is_valid(block_t const *block, block_t const *prev_block);

/* --- v0.3 Functions --- */
/* * NOTE: Do NOT include _block_print or _transaction_print_loop here.
 * Those are defined as STATIC in the .c file and cannot be declared here.
 */

#endif /* BLOCKCHAIN_H */
