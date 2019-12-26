#include "ft_printf.h"

double	ft_pow(double base, int_fast16_t power)
{
	if (power == 0)
		return (1);
	if (power < 0)
		return (ft_pow(1 / base, -power));
	if (power & 1)
		return (base * ft_pow(base, power - 1));
	return (ft_pow(base * base, power / 2));
}
