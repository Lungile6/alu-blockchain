#ifndef _CLI_H_
#define _CLI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../blockchain.h"
#include "../../../crypto/hblk_crypto.h"
#include "../../llist/llist.h"

/* Shared Globals */
extern llist_t *transaction_pool; /* The local pool */
extern llist_t *unspent_outputs;  /* The current UTXO list */

/* Global state for the session */
extern EC_KEY *wallet;
extern blockchain_t *blockchain;

/* Prototypes */
void cli_loop(void);
int cmd_wallet_load(char const *path);
int cmd_wallet_save(char const *path);
int cmd_send(char const *amount_str, char const *address_str);
int cmd_mine(void);
void cmd_exit(void);
int cmd_info(void);
int cmd_load(char const *path);
int cmd_save(char const *path);

#endif /* _CLI_H_ */
