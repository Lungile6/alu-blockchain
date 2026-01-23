#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>
#include <llist.h>
#include "../../crypto/hblk_crypto.h"

struct transaction_s
{
    uint8_t id[32];
    llist_t *inputs;
    llist_t *outputs;
};

/**
 * struct tx_out_s - Transaction output structure
 *
 * @amount: Amount of the transaction
 * @pub: Public key of the transaction receiver
 * @hash: Hash of the transaction output (amount + pub)
 */
typedef struct tx_out_s
{
    uint32_t    amount;
    uint8_t     pub[65];
    uint8_t     hash[32];
} tx_out_t;

typedef struct unspent_tx_out_s
{
    uint8_t     block_hash[32];
    uint8_t     tx_id[32];
    tx_out_t    out;
} unspent_tx_out_t;

/* --- Function Prototypes --- */

/* tx_out_create.c */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);
/* unspent_tx_out_create.c */
unspent_tx_out_t *unspent_tx_out_create(
	uint8_t const block_hash[SHA256_DIGEST_LENGTH],
	uint8_t const tx_id[SHA256_DIGEST_LENGTH],
	tx_out_t const *out);

#endif /* TRANSACTION_H */
