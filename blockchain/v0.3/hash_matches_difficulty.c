#include "blockchain.h"

/**
 * hash_matches_difficulty - Checks if a hash matches a difficulty
 * @hash: Hash to check
 * @difficulty: Difficulty to match
 *
 * Return: 1 if matches, 0 otherwise
 */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
	uint32_t difficulty)
{
	uint32_t i, j;
	uint32_t bit_count = 0;

	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		for (j = 0; j < 8; j++)
		{
			if (hash[i] & (1 << (7 - j)))
				return (bit_count >= difficulty);
			bit_count++;
		}
	}
	return (bit_count >= difficulty);
}
