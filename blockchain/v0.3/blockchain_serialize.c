#include "blockchain.h"
#include <string.h>
#include <stdio.h>

uint8_t	_get_endianness(void);


int write_unspent(llist_node_t unspent, unsigned int index, void *arg)
{
	FILE *file;
	unspent_tx_out_t *utx = unspent;
	(void)index;
	if (!arg || !unspent)
		return (-1);
	
	file = (FILE *)arg;
	
	fwrite((void *)&utx->block_hash, sizeof(utx->block_hash), 1, file);
	fwrite((void *)&utx->tx_id, sizeof(utx->tx_id), 1, file);
	fwrite((void *)&utx->out.amount, sizeof(utx->out.amount), 1, file);
	fwrite((void *)&utx->out.pub, sizeof(utx->out.pub), 1, file);
	fwrite((void *)&utx->out.hash, sizeof(utx->out.hash), 1, file);
	
	return (0);
}

int write_tx_inputs(llist_t *list, FILE *file)
{
	tx_in_t *in;
	int i, n_inputs;
	
	n_inputs = llist_size(list);
	for (i = 0; i < n_inputs; i++)
	{
		in = llist_get_node_at(list, i);
		fwrite((void *)&in->block_hash, sizeof(in->block_hash), 1, file);
		fwrite((void *)&in->tx_id, sizeof(in->tx_id), 1, file);
		fwrite((void *)&in->tx_out_hash, sizeof(in->tx_out_hash), 1, file);
		fwrite((void *)&in->sig.sig, sizeof(in->sig.sig), 1, file);
		fwrite((void *)&in->sig.len, sizeof(in->sig.len), 1, file);
	}
	return (0);
}

int write_tx_outputs(llist_t *list, FILE *file)
{
	tx_out_t *out;
	int i, n_outputs;
	
	n_outputs = llist_size(list);
	
	for (i = 0; i < n_outputs; i++)
	{
		out = llist_get_node_at(list, i);
		fwrite((void *)&out->amount, sizeof(out->amount), 1, file);
		fwrite((void *)&out->pub, sizeof(out->pub), 1, file);
		fwrite((void *)&out->hash, sizeof(out->hash), 1, file);
	}
	return (0);
}


int write_transactions(llist_t *list, FILE *file)
{
	
	transaction_t *tx;
	int i, n_txs, n_inputs, n_outputs;
	
	n_txs = llist_size(list);
	for (i = 0; i < n_txs; i++)
	{
		tx = llist_get_node_at(list, i);
		n_inputs = llist_size(tx->inputs);
		n_outputs = llist_size(tx->outputs);

		fwrite((void *)&tx->id, sizeof(tx->id), 1, file);
		fwrite(&n_inputs, sizeof(n_inputs), 1, file);
		fwrite((void *)&n_outputs, sizeof(n_outputs), 1, file);
		
		write_tx_inputs(list, file);
		write_tx_outputs(list, file);

	}
	return 0;
}
/**
 * write_block - write a block into a file
 * @block: the block to write
 * @index: the block index
 * @arg: file
 * Return: 0 on success or -1 on failure
 */
int write_block(llist_node_t block, unsigned int index, void *arg)
{
	FILE *file;
	block_t *ptr_block = block;
	int num_txs = llist_size(ptr_block->transactions);
	(void) index;
	if (!arg || !block)
		return (-1);

	file = (FILE *)arg;
	fwrite((void *)&ptr_block->info, sizeof(ptr_block->info), 1, file);
	fwrite((void *)&ptr_block->data.len, sizeof(ptr_block->data.len), 1, file);
	fwrite(ptr_block->data.buffer, ptr_block->data.len, 1, file);
	fwrite(ptr_block->hash, sizeof(ptr_block->hash), 1, file);
	fwrite(&num_txs, sizeof(num_txs), 1, file);

	write_transactions(ptr_block->transactions, file);		
	
	return (0);
}


/**
 * blockchain_serialize - serializes a Blockchain into a file
 * @blockchain: the Blockchain to be serialized
 * @path: the path to a file to serialize the Blockchain into
 * Return: 0 on success or -1 on failure
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *file;
	blockchain_file_header_t header;

	if (!blockchain || !path)
		return (-1);

	memcpy(header.magic, "HBLK", 4);
	memcpy(header.version, "0.3", 3);
	header.endian = _get_endianness();
	header.n_blocks = llist_size(blockchain->chain);
	header.n_unspent = llist_size(blockchain->unspent);
	file = fopen(path, "w");
	if (!file)
		return (-1);

	fwrite(&header, sizeof(header), 1, file);

	if (llist_for_each(blockchain->chain, write_block, file) == -1)
	{
		fclose(file);
		return (-1);
	}

	if (llist_for_each(blockchain->unspent, write_unspent, file) == -1)
	{
		fclose(file);
		return (-1);
	}
	fclose(file);
	return (0);
}
