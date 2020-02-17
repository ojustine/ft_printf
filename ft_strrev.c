#include "ft_printf.h"

void	ft_strrev(char *begin, char *end)
{
	char tmp;

	while (end > begin)
	{
		tmp = *end;
		*end-- = *begin;
		*begin++ = tmp;
	}
}
