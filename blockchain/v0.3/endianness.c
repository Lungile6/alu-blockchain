#include <stdint.h>

/**
 * _get_endianness - Determines the endianness of the host system
 *
 * Return: 1 for little-endian, 2 for big-endian
 */
uint8_t _get_endianness(void)
{
	uint32_t i = 0x01020304;

	return (((uint8_t *)&i)[0] == 0x04 ? 1 : 2);
}
