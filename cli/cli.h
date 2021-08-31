#ifndef _CLI_
#define _CLI_

#include <stddef.h>
#include "../crypto/hblk_crypto.h"
#include "../blockchain/v0.3/blockchain.h"
#include "../blockchain/v0.3/transaction/transaction.h"

typedef struct state_s
{
	int status;
	EC_KEY *wallet;
	blockchain_t *blockchain;
	llist_t *tx_pool;
	
	
} state_t;

typedef int (*command_func_t)(state_t *, int argc, char *argv[]);

typedef struct command_s
{
	
	command_func_t func;
	char const *name;
	
} command_t;


int wallet_save(state_t *state, int argc, char *argv[]);
int wallet_load(state_t *state, int argc, char *argv[]);
int send_cmd(state_t *state, int argc, char *argv[]);


void state_init(state_t *state);
void state_clear(state_t *state);
command_t *find_command(char *name);
#endif
