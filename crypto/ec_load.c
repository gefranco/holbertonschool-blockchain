#include "hblk_crypto.h"

EC_KEY *ec_load(char const *folder)
{
	FILE *file;
	char path[180];

	EC_KEY *key = NULL;

	if (!folder)
		return (NULL);

	sprintf(path, "%s/%s", folder, "key.pem");
	file = fopen(path, "r");

	if (!file)
	{
		EC_KEY_free(key);
		return (NULL);
	}

	if (!PEM_read_ECPrivateKey(file, &key, NULL, NULL))
	{
		EC_KEY_free(key);
		fclose(file);
		return (NULL);
	}
	fclose(file);

	return (key);

}
