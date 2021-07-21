#include "hblk_crypto.h"
/**
 * sha256 - computes the hash of a sequence of bytes
 * @s: the sequence of bytes to be hashed
 * @len: the number of bytes to hash in s
 * @digest: digest
 * Return: a pointer to digest or NULL on fail
 */
uint8_t *sha256(int8_t const *s,
		size_t len, uint8_t digest[SHA256_DIGEST_LENGTH])
{
	SHA256_CTX context;

	if (!SHA256_Init(&context))
		return (NULL);

	if (!SHA256_Update(&context, s, len))
		return (NULL);
	if (!SHA256_Final(digest, &context))
		return (NULL);

	return (digest);
}
