#include <stdio.h>
#include <stdlib.h>
#include "hblk_crypto.h"

/**
 * ec_load - Loads an EC key pair from the disk
 * @folder: Path to the folder from which to load the keys
 *
 * Return: Pointer to the created EC_KEY structure, or NULL upon failure
 */
EC_KEY *ec_load(char const *folder)
{
	FILE *fp;
	EC_KEY *key = NULL;
	char path[512];

	if (!folder)
		return (NULL);

	/* 1. Load the Public Key to initialize the EC_KEY and Group */
	sprintf(path, "%s/key_pub.pem", folder);
	fp = fopen(path, "r");
	if (!fp)
		return (NULL);

	/* PEM_read_EC_PUBKEY will allocate 'key' if it is NULL */
	if (!PEM_read_EC_PUBKEY(fp, &key, NULL, NULL))
	{
		fclose(fp);
		return (NULL);
	}
	fclose(fp);

	/* 2. Load the Private Key into the existing EC_KEY structure */
	sprintf(path, "%s/key.pem", folder);
	fp = fopen(path, "r");
	if (!fp)
	{
		EC_KEY_free(key);
		return (NULL);
	}

	/* Use the already allocated 'key' to store the private component */
	if (!PEM_read_ECPrivateKey(fp, &key, NULL, NULL))
	{
		fclose(fp);
		EC_KEY_free(key);
		return (NULL);
	}
	fclose(fp);

	return (key);
}
