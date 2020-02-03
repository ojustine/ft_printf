#include "ft_printf.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	void			*ret;
	unsigned char	*dest;
	unsigned char	*source;

	ret = dst;
	dest = (unsigned char*)dst;
	source = (unsigned char*)src;
	if (dest == NULL && source == NULL)
		return (NULL);
	while (n >= 8)
	{
		*((uint64_t*)dest) = *((uint64_t*)source);
		dest += 8;
		source += 8;
		n -= 8;
	}
	while (n--)
		*dest++ = *source++;
	return (ret);
}
