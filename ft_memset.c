#include <stdlib.h>

void	*ft_memset(void *b, int c, size_t n)
{
	unsigned char		*mem;
	unsigned long long	stamp_64;
	unsigned long long	*ptr_64;

	mem = (unsigned char*)b;
	if (n >= 8)
	{
		stamp_64 = 0xff & c;
		stamp_64 = (stamp_64 << 0x8) | stamp_64;
		stamp_64 = (stamp_64 << 0x10) | stamp_64;
		stamp_64 = (stamp_64 << 0x20) | stamp_64;
		ptr_64 = (unsigned long long*)mem;
		while (n >= 8)
		{
			*ptr_64++ = stamp_64;
			mem += 8;
			n -= 8;
		}
	}
	while (n--)
		*mem++ = (unsigned char)c;
	return (b);
}

