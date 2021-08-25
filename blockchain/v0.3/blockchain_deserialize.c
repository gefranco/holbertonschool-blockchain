#include "blockchain.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
void _swap_endian(void *p, size_t size);
int8_t	_get_endianness(void);

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
	fclose(file);
	return (blockchain);

}
