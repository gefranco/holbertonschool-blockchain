#include "hblk_crypto.h"
#include <strings.h>
uint8_t *ec_sign(EC_KEY const *key,
			uint8_t const *msg, size_t msglen, sig_t *sig)
{
	uint32_t len = 0;

	if (!key || !msg)
		return (NULL);

	bzero(sig->sig, sizeof(sig->sig));
	if (!ECDSA_sign(0, msg, msglen, sig->sig, &len, (EC_KEY *)key))
	{
		sig->len = 0;
		return (NULL);
	}
	sig->len = (uint8_t)len;
	return ((uint8_t *)sig->sig);
}
