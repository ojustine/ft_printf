#include "ft_printf.h"

int32_t	ft_divmod(int32_t dividend, int32_t divisor, int32_t *remainder)
{
	*remainder = dividend % divisor;
	return (dividend / divisor);
}
