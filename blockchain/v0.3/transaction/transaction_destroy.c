#include "transaction.h"
/**
 * transaction_destroy - deallocates a transaction structure
 * @transaction: points to the transaction to delete
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */

void transaction_destroy(transaction_t *transaction)
{
	if (!transaction)
		return;

	if (transaction->inputs) llist_destroy(transaction->inputs, 1, free);
	if (transaction->outputs) llist_destroy(transaction->outputs, 1, free);
	free(transaction);
}
