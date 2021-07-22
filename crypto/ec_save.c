#include "hblk_crypto.h"
#include <sys/stat.h>

/**
 * ec_save - saves an existing EC key pair on the disk
 *
 * @key: points to the EC key pair to be saved on disk
 * @folder: the path to the folder in which to save the keys
 *
 * Return: 1 on success or 0 on FAILURE
 */
int ec_save(EC_KEY *key, char const *folder)
{
	FILE *file;
	struct stat st = {0};
	char path[120];

	if (!key || !folder)
		return (0);

	if (stat(folder, &st) == -1)
	{
		mkdir(folder, 0700);
	}

	sprintf(path, "%s/%s", folder, "key.pem");

	file = fopen(path, "w");

	if (!file)
		return (0);

	if (!PEM_write_ECPrivateKey(file, key, NULL, NULL, 0, NULL, NULL))
	{
		fclose(file);
		return (0);
	}
	fclose(file);


	sprintf(path, "%s/%s", folder, "key_pub.pem");
	file = fopen(path, "w");
	if (!file)
		return (0);

	if (!PEM_write_EC_PUBKEY(file, key))
	{
		fclose(file);
		return (0);
	}
	fclose(file);
	return (1);
}
