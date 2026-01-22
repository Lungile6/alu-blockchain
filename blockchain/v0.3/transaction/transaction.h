#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>
#include "../../crypto/hblk_crypto.h"

/**
 * struct tx_out_s - Transaction output structure
 *
 * @amount: Amount of the transaction
 * @pub: Public key of the transaction receiver
 * @hash: Hash of the transaction output (amount + pub)
 */
typedef struct tx_out_s
{
	uint32_t amount;
	uint8_t pub[EC_PUB_LEN];
	uint8_t hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/* --- Function Prototypes --- */

/* tx_out_create.c */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

#endif /* TRANSACTION_H */
