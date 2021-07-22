#include "hblk_crypto.h"
int ec_verify(EC_KEY const *key,
		uint8_t const *msg, size_t msglen, sig_t const *sig)
{
	if (!key || !msg || !sig)
		return (0);
	if (!ECDSA_verify(0, msg, msglen, sig->sig, sig->len, (EC_KEY *)key))
		return (0);
	return (1);
}
