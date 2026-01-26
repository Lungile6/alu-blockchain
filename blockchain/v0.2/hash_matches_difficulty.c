#include "blockchain.h"

/**
 * hash_matches_difficulty - Checks if a hash matches a given difficulty
 * @hash: The hash to check
 * @difficulty: The minimum number of leading zero bits required
 *
 * Return: 1 if the difficulty is respected, 0 otherwise
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
			    uint32_t difficulty)
{
	uint32_t i, j, count = 0;

	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (hash[i] & (1 << (7 - j)))
				return (count >= difficulty);
			count++;
		}
	}
	return (count >= difficulty);
}
