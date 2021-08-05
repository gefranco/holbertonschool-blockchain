#include "blockchain.h"
#include <stdio.h>
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
				uint32_t difficulty)
{
	int i;
	int dif = 1;
	uint8_t *ptr = (uint8_t *)hash;
	uint32_t difficulty_calc = 0;

	while (ptr < hash + SHA256_DIGEST_LENGTH && dif)
	{
		for (i = 7; i >= 0; i--)
		{

			if ((*ptr >> i) & 1)
			{
				dif = 0;
				break;
			}
			difficulty_calc++;
		}
		ptr++;
	}

	if (difficulty_calc < difficulty)
		return (0);
	return (1);
}
