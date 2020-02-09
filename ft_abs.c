#include "ft_printf.h"

uint32_t	ft_abs(int32_t n)
{
	return ((n + (n >> 31)) ^ (n >> 31));
}
