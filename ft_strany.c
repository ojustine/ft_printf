#include "ft_printf.h"

int	ft_strany(char const *str, int32_t c)
{
	if (str == NULL)
		return (0);
	if ((unsigned char)c == '\0')
		return (1);
	while (*str)
	{
		if (*str == (unsigned char)c)
			return (1);
		str++;
	}
	return (0);
}
