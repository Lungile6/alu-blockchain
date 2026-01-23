#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <time.h>
#include <llist.h>

/* --- FORWARD DECLARATIONS --- */
/* This tells the compiler transaction_t is a struct that exists somewhere */
struct transaction_s;
typedef struct transaction_s transaction_t;

/* 1. Include the transaction header FIRST */
/* This defines transaction_t, tx_in_t, tx_out_t, etc. */
#include "transaction/transaction.h"
#include "../../crypto/hblk_crypto.h"

/* Constants */
#define SHA256_DIGEST_LENGTH 32
#define BLOCKCHAIN_DATA_MAX 1024

/* 2. Struct Definitions */

typedef struct block_info_s
{
	uint32_t    index;
	uint32_t    difficulty;
	uint64_t    timestamp;
	uint64_t    nonce;
	uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/* Printer expects this to exist */
typedef struct block_data_s
{
	int8_t      buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t    len;
} block_data_t;

typedef struct block_s
{
	block_info_t    info;
	block_data_t    data;         /* For v0.1 printer compatibility */
	llist_t         *transactions; /* For v0.3 logic */
	uint8_t         hash[SHA256_DIGEST_LENGTH];
} block_t;

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

/* Printer Prototypes: transaction_t is now known here */
void _blockchain_print(blockchain_t const *blockchain);
int _transaction_print_loop(transaction_t const *transaction, unsigned int idx, char const *indent);

#endif /* BLOCKCHAIN_H */
