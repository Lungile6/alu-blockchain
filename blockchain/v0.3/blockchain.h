#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <time.h>
#include <llist.h>

/* 1. Include transaction.h FIRST so transaction_t is known */
#include "transaction/transaction.h"
#include "../../crypto/hblk_crypto.h"

/* Constants */
#define SHA256_DIGEST_LENGTH 32
#define BLOCKCHAIN_DATA_MAX 1024

/* --- Struct Definitions --- */

typedef struct block_info_s
{
	uint32_t    index;
	uint32_t    difficulty;
	uint64_t    timestamp;
	uint64_t    nonce;
	uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/* The printer expects this struct to exist */
typedef struct block_data_s
{
	int8_t      buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t    len;
} block_data_t;

typedef struct block_s
{
	block_info_t    info;
	block_data_t    data;         /* Keep this for the printer's sake */
	llist_t         *transactions; /* The v0.3 addition */
	uint8_t         hash[SHA256_DIGEST_LENGTH];
} block_t;

typedef struct blockchain_s
{
	llist_t     *chain;
	llist_t     *unspent;
} blockchain_t;

/* --- Prototypes for the Printer --- */
/* Note: These must match the signatures in _blockchain_print.c exactly */
void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_print_brief(blockchain_t const *blockchain);

/* Note: These are 'static' in the .c file, so we don't declare them here 
   unless they are called by other files. */

#endif /* BLOCKCHAIN_H */
