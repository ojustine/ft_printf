#include "ft_printf.h"

intmax_t	ft_moddiv(intmax_t dividend, intmax_t divisor, intmax_t *quotient)
{
	*quotient = dividend / divisor;
	return (dividend % divisor);
}
