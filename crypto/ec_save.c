#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "hblk_crypto.h"

/**
 * ec_save - Saves an existing EC key pair on the disk
 * @key: Points to the EC key pair to be saved
 * @folder: Path to the folder in which to save the keys
 *
 * Return: 1 upon success, 0 upon failure
 */
int ec_save(EC_KEY *key, char const *folder)
{
	char path[512];
	FILE *fp;

	if (!key || !folder)
		return (0);

	/* 1. Create the directory if it doesn't exist */
	if (mkdir(folder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
	{
		if (errno != EEXIST)
			return (0);
	}

	/* 2. Save Private Key to <folder>/key.pem */
	snprintf(path, sizeof(path), "%s/key.pem", folder);
	fp = fopen(path, "w");
	if (!fp)
		return (0);
	if (!PEM_write_ECPrivateKey(fp, key, NULL, NULL, 0, NULL, NULL))
	{
		fclose(fp);
		return (0);
	}
	fclose(fp);

	/* 3. Save Public Key to <folder>/key_pub.pem */
	snprintf(path, sizeof(path), "%s/key_pub.pem", folder);
	fp = fopen(path, "w");
	if (!fp)
		return (0);
	if (!PEM_write_EC_PUBKEY(fp, key))
	{
		fclose(fp);
		return (0);
	}
	fclose(fp);

	return (1);
}
