#include "ft_printf.h"

intmax_t	ft_divmod(intmax_t dividend, intmax_t divisor, intmax_t *remainder)
{
	*remainder = dividend % divisor;
	return (dividend / divisor);
}
