#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "cli.h"

static int send(state_t *state, uint32_t amount, uint8_t pub[EC_PUB_LEN], char *argv[])
{
	EC_KEY *receiver = NULL;
	transaction_t *tx = NULL;
	
	receiver = ec_from_pub(pub);
	
	if (!receiver)
	{
		fprintf(stderr, "%s: %s: invalid receiver public key\n",
			argv[0], argv[1]);
		return((state->status = EXIT_FAILURE));
	}
	tx = transaction_create(state->wallet,
		receiver,
		amount,
		state->blockchain->unspent);
	if (!tx)
	{
		fprintf(stderr, "%s: failed to create transaction\n",
			argv[0]);
		EC_KEY_free(receiver);
		return ((state->status = EXIT_FAILURE));
	}

	if ((transaction_is_valid(tx, state->blockchain->unspent)) != 0)
	{
		fprintf(stderr, "%s: invalid transaction\n",
			argv[0]);
		EC_KEY_free(receiver);
		transaction_destroy(tx);
		return((state->status = EXIT_FAILURE));
	}

	if (llist_add_node(state->tx_pool, tx, ADD_NODE_REAR) == -1)
	{
		fprintf(stdout, "Failed to add transaction to local transaction pool\n");
		EC_KEY_free(receiver);
		transaction_destroy(tx);
		return ((state->status = EXIT_FAILURE));
	}
	
	fprintf(stdout, "Transaction added to local transaction pool\n");
	EC_KEY_free(receiver);

	return ((state->status = EXIT_SUCCESS));

}


int send_cmd(state_t *state, int argc, char *argv[])
{
	uint8_t pub[EC_PUB_LEN] = {0};
	size_t pub_index = 0;
	unsigned long int amount = 0;
	char *endptr = NULL;
	int nmatched = 0;
	if (argc > 3)
	{
		fprintf(stderr, "%s: too many arguments\n", argv[0]);
		return ((state->status = 2));
	}

	if (argc < 3)
	{
		fprintf(stderr, "%s: too few arguments\n", argv[0]);
		return ((state->status = 2));
	}
	errno = 0;
	amount = strtoul(argv[1], &endptr, 0);
	if ((errno != 0 && (amount == 0 || amount == ULONG_MAX)) || 
		*endptr != '\0'|| 
		amount > UINT32_MAX)
	{
		fprintf(stderr, "%s: %s: Invalid amount\n",
			argv[0], argv[1]);
		return ((state->status = EXIT_FAILURE));
	}

	for (pub_index = 0; pub_index < EC_PUB_LEN; pub_index += 1)
	{
		nmatched = sscanf((argv[2] + (2 * pub_index)), "%02x",
				(unsigned int *)(pub + pub_index));
		if (nmatched == 0)
			break; 
	}

	return (send(state, amount, pub, argv));
}
