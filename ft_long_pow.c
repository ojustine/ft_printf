#include "ft_printf.h"

long double	ft_long_pow(long double base, int_fast16_t power)
{
	if (power == 0)
		return (1);
	if (power < 0)
		return (ft_long_pow(1 / base, -power));
	if (power & 1)
		return (base * ft_long_pow(base, power - 1));
	return (ft_long_pow(base * base, power / 2));
}
