#include "ft_printf.h"

int32_t	ft_moddiv(int32_t dividend, int32_t divisor, int32_t *quotient)
{
	*quotient = dividend / divisor;
	return (dividend % divisor);
}
