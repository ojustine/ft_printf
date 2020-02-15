#include "ft_printf.h"

size_t	ft_wstrlen(wint_t *s)
{
	size_t	len;

	len = 0;
	while (*s != L'\0')
	{
		len++;
		len += *s > 0xFFFF;
		len += *s > 0x7FF;
		len += *s > 0x7F;
		++s;
	}
	return (len);
}

