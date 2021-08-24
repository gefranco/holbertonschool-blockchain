#ifndef _BLOCKCHAIN_DS_
#define _BLOCKCHAIN_DS_

#define BLOCKCHAIN_DATA_MAX	1024
#define BLOCK_GENERATION_INTERVAL 1
#define DIFFICULTY_ADJUSTMENT_INTERVAL 5

#define GENESIS_HASH "\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"

#include <openssl/sha.h>
#include "transaction/transaction.h"
/**
 * struct blockchain_s - Blockchain structure
 *
 * @chain: Linked list of pointers to block_t
 */
typedef struct blockchain_s
{
	llist_t	*chain;
	llist_t *unspent;
} blockchain_t;

/**
 * struct block_info_s - Block info structure
 *
 * @index:      Index of the Block in the Blockchain
 * @difficulty: Difficulty of proof of work (hash leading zero bits)
 * @timestamp:  Time the Block was created at (UNIX timestamp)
 * @nonce:      Salt value used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the Blockchain
 */
typedef struct block_info_s
{
	uint32_t	index;
	uint32_t	difficulty;
	uint64_t	timestamp;
	uint64_t	nonce;
	uint8_t		prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_data_s - Block data
 *
 * @buffer: Data buffer
 * @len:    Data size (in bytes)
 */
typedef struct block_data_s
{
	int8_t buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t        len;
} block_data_t;

/**
 * struct block_s - Block structure
 *
 * @info: Block info
 * @data: Block data
 * @transactions: List of transactions
 * @hash: 256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
	block_info_t	info;
	block_data_t	data;
	llist_t		*transactions;
	uint8_t		hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_file_header_s - File Header
 *
 * @magic: magic bytes for blockchain file format
 * @version: Blockchain version
 * @endian: 1 litle endian, 2 big endian
 * @n_blocks: number of blocks
 */
typedef struct blockchain_file_header_s
{
	uint8_t magic[4];
	uint8_t version[3];
	uint8_t endian;
	int32_t n_blocks;

} blockchain_file_header_t;


blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev,
			int8_t const *data, uint32_t data_len);
void block_destroy(block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
uint8_t *block_hash(block_t const *block,
			uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
blockchain_t *blockchain_deserialize(char const *path);
int block_is_valid(block_t const *block, block_t const *prev_block, llist_t *all_unspent);
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH], uint32_t difficulty);
void block_mine(block_t *block);
uint32_t blockchain_difficulty(blockchain_t const *blockchain);
#endif
