#include "ft_printf.h"

size_t	ft_intlen(int32_t n)
{
	const static int32_t	sizes[10] = {9, 99, 999, 9999, 99999, 999999,
							9999999, 99999999, 999999999, INT32_MAX};
	register size_t			i;

	if (n < 0)
	{
		if (n == INT32_MIN)
			return (10);
		else
			n = -n;
	}
	i = 0;
	while (i++ < 11)
		if (n <= sizes[i - 1])
			return (i);
}
