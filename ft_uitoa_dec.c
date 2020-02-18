#include "libptf.h"

size_t	ft_uitoa_dec(const uint32_t val, char *buff)
{
	register uint32_t		lo;
	register uint32_t		hi;
	register uint32_t		mask;
	register uint32_t		shift;
	register int_fast16_t	i;

	lo = (val % 100000) * ((1 << 28) / 10000 + 1) - (val % 100000 / 4);
	hi = (val / 100000) * ((1 << 28) / 10000 + 1) - (val / 100000 / 4);
	mask = 0x0fffffff;
	shift = 29;
	i = -1;
	while (++i < 5 && --shift)
	{
		buff[4 - i] = (char)('0' + (lo >> shift));
		buff[9 - i] = (char)('0' + (hi >> shift));
		hi = (hi & mask) * 5;
		lo = (lo & mask) * 5;
		mask >>= 1;
	}
	i = 1;
	while (buff[i] != '0' && i < 10)
		i++;
	ft_strrev(buff, buff + i - 1);
	buff[i] = 0;
	return (i);
}
