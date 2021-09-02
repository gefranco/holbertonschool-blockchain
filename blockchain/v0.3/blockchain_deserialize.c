#include "blockchain.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void _swap_endian(void *p, size_t size);
int8_t	_get_endianness(void);


llist_t *read_outputs(FILE *file, uint8_t swap_endian, int num_outputs)
{
	llist_t *outputs;
	tx_out_t *out;
	
	if (num_outputs <= 0)
		return NULL;
	outputs = llist_create(MT_SUPPORT_FALSE);
	
	if (!outputs)
		return (NULL);

	while ((int) num_outputs-- > 0)
	{
		out = calloc(1, sizeof(tx_out_t));
		llist_add_node(outputs, out, ADD_NODE_REAR);
		fread(&out->amount, sizeof(out->amount), 1, file);
		fread(&out->pub, sizeof(out->pub), 1, file);
		fread(&out->hash, sizeof(out->hash), 1, file);
		if (swap_endian)
			_swap_endian(&out->amount, sizeof(out->amount));
	}

	return (outputs);
}

llist_t *read_inputs(FILE *file, uint8_t swap_endian, int num_inputs)
{
	llist_t *inputs;
	tx_in_t *in;
	if(num_inputs <= 0)
		return NULL;
	
	inputs = llist_create(MT_SUPPORT_FALSE);
	if (!inputs)
		return NULL;
	
	while ((int)num_inputs-- > 0)
	{
		in = calloc(1, sizeof(tx_in_t));
		llist_add_node(inputs, in, ADD_NODE_REAR);
		
		fread(&in->block_hash, sizeof(in->block_hash), 1, file);
		fread(&in->tx_id, sizeof(in->tx_id), 1, file);
		fread(&in->tx_out_hash, sizeof(in->tx_out_hash), 1, file);
		fread(&in->sig.sig, sizeof(in->sig.sig), 1, file);
		fread(&in->sig.len, sizeof(in->sig.len), 1, file);
		if (swap_endian)
                        _swap_endian(&in->sig.len, sizeof(in->sig.len));
	}
	return inputs;
}

llist_t *read_transactions(FILE *file, uint8_t swap_endian)
{
	uint32_t num_txs, num_inputs, num_outputs;
	transaction_t *tx;
	llist_t *list;
	
	fread(&num_txs, sizeof(num_txs), 1, file);

	list = llist_create(MT_SUPPORT_FALSE);
	
	if(!list)
		return (NULL);
	if (swap_endian)
		_swap_endian(&num_txs, sizeof(num_txs));
	if((int)num_txs <= 0)
	{
		llist_destroy(list, 0, NULL);
		return (NULL);
	}
	
	while (num_txs-- > 0)
	{
		tx = calloc(1, sizeof(transaction_t));
		llist_add_node(list, tx, ADD_NODE_REAR);
		fread(&tx->id, sizeof(tx->id), 1, file);
		fread(&num_inputs, sizeof(num_inputs),1, file);
		if (swap_endian)
			_swap_endian(&num_inputs, sizeof(num_inputs));
		fread(&num_outputs, sizeof(num_outputs), 1, file);
		if (swap_endian)
			_swap_endian(&num_outputs, sizeof(num_outputs));
		
		
		tx->inputs = read_inputs(file, swap_endian, num_inputs);
		tx->outputs = read_outputs(file, swap_endian, num_outputs);
	}	
	return (list);
}

int read_unspent_txs(FILE *file, uint8_t swap_endian,
                        blockchain_t *blockchain, int size)
{
	int i;
        unspent_tx_out_t *utxo;
        llist_t *list = llist_create(MT_SUPPORT_TRUE);

        for (i = 0; i < size; i++)
        {
                utxo = calloc(1, sizeof(unspent_tx_out_t));
                if (!utxo)
                        return (0);
                fread(&utxo->block_hash, sizeof(utxo->block_hash), 1, file);
                fread(&utxo->tx_id, sizeof(utxo->tx_id), 1, file);
                fread(&utxo->out.amount, sizeof(utxo->out.amount), 1, file);
		fread(&utxo->out.pub, sizeof(utxo->out.pub), 1, file);
                fread(&utxo->out.hash, sizeof(utxo->out.hash), 1, file);

		if (swap_endian)
                        _swap_endian(&utxo->out.amount, sizeof(utxo->out.amount));
                llist_add_node(list, utxo, ADD_NODE_REAR);

        }
        blockchain->unspent = list;
        return (1);
}

/**
 * read_blocks - Entry point
 * @file: pointer to the file
 * @swap_endian: if swap is needed
 * @blockchain: the blokchain
 * @size: number of blocks to read
 * Return: 1 on success, 0 on failure
 */
int read_blocks(FILE *file, uint8_t swap_endian,
			blockchain_t *blockchain, int size)
{
	int i;
	block_t *block;
	uint32_t data_len;
	llist_t *list = llist_create(MT_SUPPORT_TRUE);

	for (i = 0; i < size; i++)
	{
		block = calloc(1, sizeof(block_t));
		if (!block)
			return (0);
		fread(&block->info, sizeof(block->info), 1, file);
		fread(&data_len, sizeof(data_len), 1, file);
		if (swap_endian)
			_swap_endian(&data_len, sizeof(data_len));
		fread(&block->data.buffer, data_len, 1, file);
		fread(&block->hash, SHA256_DIGEST_LENGTH, 1, file);

		if (swap_endian)
		{
			_swap_endian(&block->info.index, sizeof(block->info.index));
			_swap_endian(&block->info.difficulty, sizeof(block->info.difficulty));
			_swap_endian(&block->info.timestamp, sizeof(block->info.timestamp));
			_swap_endian(&block->info.nonce, sizeof(block->info.nonce));
		}
		block->data.len = data_len;
		block->transactions = read_transactions(file, swap_endian);
	
		llist_add_node(list, block, ADD_NODE_REAR);

	}
	blockchain->chain = list;
	return (1);
}

/**
 * blockchain_deserialize - deserializes a Blockchain from a file
 * @path: the path to a file
 * Return: the blockchain or NULL on failure
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *file;
	blockchain_file_header_t header;
	blockchain_t *blockchain = NULL;
	int size;
	uint8_t endian_swap;

	if (!path)
		return (NULL);
	if (access(path, R_OK) == -1)
	{
		return (NULL);
	}
	file = fopen(path, "r");
	if (!file)
		return (NULL);

	fread(&header, sizeof(header), 1, file);

	blockchain = calloc(1, sizeof(blockchain_t));
	if (!blockchain)
	{
		fclose(file);
		return (NULL);
	}
	endian_swap = _get_endianness() != header.endian;
	size = header.n_blocks;
	if (endian_swap)
		_swap_endian(&size, sizeof(size));

	if (!read_blocks(file, endian_swap, blockchain, size))
	{
		blockchain_destroy(blockchain);
		fclose(file);
		return (NULL);
	}

	size = header.n_unspent;
	if (endian_swap)
                _swap_endian(&size, sizeof(size));

	if (!read_unspent_txs(file, endian_swap, blockchain, size))
        {
                blockchain_destroy(blockchain);
                fclose(file);
                return (NULL);
        }

	fclose(file);
	return (blockchain);

}
