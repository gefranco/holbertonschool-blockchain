#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "cli.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);
/**
 * find_unspent_output - finds unspent output
 * @all_unspent: unspent tx ouput list
 * @tx_in: transaction input
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
tx_in_t *find_unspent_output_in_pool(llist_t *all_unspent, tx_in_t *tx_in)
{
        int i, size;
        tx_in_t *utx_out;
	

        size = llist_size(all_unspent);
        for (i = 0; i < size; i++)
        {
                utx_out = llist_get_node_at(all_unspent, i);

		if (memcmp(utx_out->tx_out_hash,
                        tx_in->tx_out_hash,
                        sizeof(utx_out->tx_out_hash)) == 0 &&
                        memcmp(utx_out->block_hash,
                                tx_in->block_hash,
                                sizeof(utx_out->block_hash)) == 0
                )
                        return (utx_out);
        }
        return (NULL);
}

/**
 * send - send coins
 * @state: cli state
 * @amount: total coins to send
 * @pub: public key to send the coins
 * @argv: arguments
 * Return: EXIT_FAILURE if call to a function fails
 *         EXIT_SUCCESS otherwise
 */
static int send(state_t *state,
		uint32_t amount,
		uint8_t pub[EC_PUB_LEN], char *argv[])
{
	EC_KEY *receiver = NULL;
	transaction_t *tx = NULL;
	
	transaction_t *tx_find = NULL;
	tx_in_t *utx_out;
	int i, i_2, size, size_inputs;
	tx_in_t *tx_in;


	receiver = ec_from_pub(pub);
	printf("sending\n");

	if (!receiver)
	{
		fprintf(stderr, "%s: %s: invalid receiver public key\n",
			argv[0], argv[1]);
		return ((state->status = EXIT_FAILURE));
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
	if (transaction_is_valid(tx, state->blockchain->unspent) != 1)
	{
		fprintf(stderr, "%s: invalid transaction\n",
			argv[0]);
		EC_KEY_free(receiver);
		transaction_destroy(tx);
		return ((state->status = EXIT_FAILURE));
	}

	/*validate there is no double tx with the same utxout*/
	printf("****new validation***\n");
	size = llist_size(state->tx_pool);
        for (i = 0; i < size; i++)
        {
                tx_find = llist_get_node_at(state->tx_pool, i);
		

		size_inputs = llist_size(tx_find->inputs);
        	for (i_2 = 0; i_2 < size_inputs; i_2++)
        	{
                	tx_in = llist_get_node_at(tx_find->inputs, i_2);
                	utx_out = find_unspent_output_in_pool(tx->inputs, tx_in);

                	if (utx_out)
                	{
				fprintf(stderr, "Double spending found\n");
                        	return ((state->status = EXIT_FAILURE));

                	}

        	}	

        }
       
	
	/**/
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

/**
 * send_cmd - send coins
 * @state: cli state
 * @argc: number of arguments passed
 * @argv: arguments
 * Return: 2 if number of arguments are wrong,
 *         EXIT_FAILURE if call to a function fails
 *         EXIT_SUCCESS otherwise
 */
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
		*endptr != '\0' ||
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
