#include "hblk_crypto.h"

/**
 * ec_to_pubxtracts the public key from an EC_KEY opaque structure
 * @key: a pointer to the EC_KEY structure
 * @pub: the address at which to store the extracted public key
 * Return: a pointer to pub or NULL on failure
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	BN_CTX *context = NULL;
	const EC_GROUP *ec_group = NULL;
	const EC_POINT *ec_point = NULL;
	size_t len = 0;

	if (!key)
		return (NULL);

	context = BN_CTX_new();
	ec_group = EC_KEY_get0_group(key);
	ec_point = EC_KEY_get0_public_key(key);

	len = EC_POINT_point2oct(ec_group, ec_point,
					(point_conversion_form_t) POINT_CONVERSION_UNCOMPRESSED,
						NULL, EC_PUB_LEN, context);
	if (!EC_POINT_point2oct(ec_group, ec_point,
					(point_conversion_form_t) POINT_CONVERSION_UNCOMPRESSED,
						(unsigned char *)pub, len, context))
		return (NULL);
	BN_CTX_free(context);

	return (pub);
}
