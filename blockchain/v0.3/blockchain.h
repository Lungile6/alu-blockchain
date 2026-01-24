#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <time.h>
#include <llist.h>

/* --- Type Compatibility Guard --- */
/* This prevents "redefinition of typedef" errors on strict intranet checkers */
#ifndef TRANSACTION_T
#define TRANSACTION_T
typedef struct transaction_s transaction_t;
#endif

/* 1. Include Transaction and Crypto headers */
#include "transaction/transaction.h"
#include "../../crypto/hblk_crypto.h"

/* --- Constants --- */
#define GENESIS_TIMESTAMP 1537578000
#define GENESIS_DATA_LEN 16
#define GENESIS_DATA "Holberton School"
#define GENESIS_HASH "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"

#define BLOCKCHAIN_DATA_MAX 1024

/* --- Struct Definitions --- */

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
 * struct block_data_s - Block data structure (v0.1 compatibility)
 */
typedef struct block_data_s
{
	int8_t      buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t    len;
} block_data_t;

/**
 * struct block_s - Block structure
 */
typedef struct block_s
{
	block_info_t    info;
	block_data_t    data;         /* Required for printer compatibility */
	llist_t         *transactions; /* v0.3 list of transaction_t */
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
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int block_is_valid(block_t const *block, block_t const *prev_block);

/* --- v0.3 Functions --- */
/* int _block_print(block_t const *block, unsigned int index, char const *prefix); */
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len);
void block_destroy(block_t *block);

/* --- Printer Prototypes --- */
/* transaction_t is recognized here because of the guard at the top */
void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_print_brief(blockchain_t const *blockchain);
int _transaction_print_loop(transaction_t const *transaction,
	unsigned int idx, char const *indent);

#endif /* BLOCKCHAIN_H */
