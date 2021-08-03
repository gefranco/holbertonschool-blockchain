#include "blockchain.h"
#include <string.h>
#include <stdio.h>

uint8_t	_get_endianness(void);

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
	(void) index;
	if (!arg || !block)
		return (-1);

	file = (FILE *)arg;
	fwrite((void *)&ptr_block->info, sizeof(ptr_block->info), 1, file);
	fwrite((void *)&ptr_block->data.len, sizeof(ptr_block->data.len), 1, file);
	fwrite(ptr_block->data.buffer, ptr_block->data.len, 1, file);
	fwrite(ptr_block->hash, sizeof(ptr_block->hash), 1, file);
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
	memcpy(header.version, "0.1", 3);
	header.endian = _get_endianness();
	header.n_blocks = llist_size(blockchain->chain);

	file = fopen(path, "w");
	if (!file)
		return (-1);

	fwrite(&header, sizeof(header), 1, file);

	if (llist_for_each(blockchain->chain, write_block, file) == -1)
	{
		fclose(file);
		return (-1);
	}
	fclose(file);
	return (0);
}
