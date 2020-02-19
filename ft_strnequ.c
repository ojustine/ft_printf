#include "libptf.h"

int	ft_strnequ(char const *s1, char const *s2, size_t n)
{
	if (s1 == NULL || s2 == NULL)
		return (0);
	while (*s1 == *s2 && n--)
	{
		if (*s1 == 0)
			return (1);
		s1++;
		s2++;
	}
	return (1);
}