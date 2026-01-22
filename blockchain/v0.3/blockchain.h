#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <time.h>
#include <llist.h>

/* * 1. Include Dependencies FIRST 
 * Note: transaction.h must be included before any prototypes 
 * that use transaction_t or tx_out_t.
 */
#include "../../crypto/hblk_crypto.h"
#include "transaction/transaction.h"

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
	llist_t         *transactions; /* transactions replaces v0.1 buffer */
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

/* --- Prototypes --- */

blockchain_t *blockchain_create(void);
void blockchain_destroy(blockchain_t *blockchain);
void block_destroy(block_t *block);
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);

/* Prototypes for provided printer functions - Adjusted to match .c file signatures */
int _block_print(block_t const *block, unsigned int index, char const *prefix);
int _transaction_print_loop(transaction_t const *transaction, int idx, char const *indent);

#endif /* BLOCKCHAIN_H */
