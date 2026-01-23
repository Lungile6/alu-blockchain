#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <time.h>
#include <llist.h>

/* 1. Include the Crypto and Transaction headers FIRST */
#include "../../crypto/hblk_crypto.h"
#include "transaction/transaction.h"

/* Constants */
#define SHA256_DIGEST_LENGTH 32
#define BLOCKCHAIN_DATA_MAX 1024

/* 2. Forward declarations to satisfy the compiler */
typedef struct block_s block_t;
typedef struct blockchain_s blockchain_t;

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
 * struct block_data_s - Block data (kept for printer compatibility)
 */
typedef struct block_data_s
{
	int8_t      buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t    len;
} block_data_t;

/**
 * struct block_s - Block structure
 */
struct block_s
{
	block_info_t    info;
	block_data_t    data;         /* Required by your _blockchain_print.c */
	llist_t         *transactions; /* The v0.3 list of transactions */
	uint8_t         hash[SHA256_DIGEST_LENGTH];
};

/**
 * struct blockchain_s - Blockchain structure
 */
struct blockchain_s
{
	llist_t     *chain;
	llist_t     *unspent;
};

/* --- Prototypes --- */
blockchain_t *blockchain_create(void);
void blockchain_destroy(blockchain_t *blockchain);
void block_destroy(block_t *block);
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);

/* Printer helpers - These now recognize transaction_t because of the include above */
void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_print_brief(blockchain_t const *blockchain);

#endif /* BLOCKCHAIN_H */
