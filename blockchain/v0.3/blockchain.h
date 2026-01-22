#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <time.h>
#include <llist.h>

/* Dependencies */
#include "../../crypto/hblk_crypto.h"
#include "transaction.h"

/* Constants */
#define GENESIS_TIMESTAMP 1537578000
#define GENESIS_DATA "Holberton School"
#define GENESIS_HASH "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"

/**
 * struct block_info_s - Block info structure
 *
 * @index:      Index of the Block in the Blockchain
 * @difficulty: Difficulty of proof of work (hash leading zero bits)
 * @timestamp:  Time the Block was created at (UNIX timestamp)
 * @nonce:      Salt value used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the Blockchain
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
 *
 * @info:         Block info
 * @transactions: List of transactions (pointers to transaction_t)
 * @hash:         256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
	block_info_t    info;
	llist_t         *transactions;
	uint8_t         hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_s - Blockchain structure
 *
 * @chain:   Linked list of pointers to block_t
 * @unspent: List of pointers to unspent_tx_out_t
 */
typedef struct blockchain_s
{
	llist_t     *chain;
	llist_t     *unspent;
} blockchain_t;

/* --- v0.1 Functions --- */

blockchain_t *blockchain_create(void);
void blockchain_destroy(blockchain_t *blockchain);
void block_destroy(block_t *block);
uint8_t *block_hash(block_t const *block,
		    uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
blockchain_t *blockchain_deserialize(char const *path);
int block_is_valid(block_t const *block, block_t const *prev_block);

/* --- v0.2 Functions (Mining/PoW) --- */

int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
			    uint32_t difficulty);
void block_mine(block_t *block);
uint32_t blockchain_difficulty(blockchain_t const *blockchain);

/* --- v0.3 Functions (Transactions) --- */

/* Note: tx_out_create was handled previously, but typically 
   the block_create prototype changes in v0.3 to use transactions */
block_t *block_create(block_t const *prev, int8_t const *data,
		      uint32_t data_len);

#endif /* BLOCKCHAIN_H */
