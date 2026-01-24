#include <stdlib.h>
#include "transaction.h"

/**
 * transaction_destroy - Deallocates a transaction structure
 * @transaction: Points to the transaction to delete
 */
void transaction_destroy(transaction_t *transaction)
{
	if (!transaction)
		return;

	/* Destroy inputs list and free all tx_in_t pointers inside */
	llist_destroy(transaction->inputs, 1, free);

	/* Destroy outputs list and free all tx_out_t pointers inside */
	llist_destroy(transaction->outputs, 1, free);

	/* Free the transaction structure itself */
	free(transaction);
}
