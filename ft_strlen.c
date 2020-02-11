#include "ft_printf.h"

size_t	ft_strlen(const char *s)
{
	register const char *tmp = s;

	if (!s)
		return (0);
	while (*tmp)
		tmp++;
	return (tmp - s);
}