#include "ft_printf.h"

static inline void	fast_dtoa_roundup(double diff, const int_fast16_t prec,
					uint32_t *whole, uint32_t *frac)
{
	static const double	powers_of_10[] = {1, 10, 100, 1000, 10000, 100000,
	1000000, 10000000, 100000000, 1000000000};

	if (diff > 0.5
	|| (diff == 0.5 && prec > 0 && (*frac & 1))
	|| (diff == 0.5 && prec == 0 && (*whole & 1)))
	{
		(*frac)++;
		if (*frac >= powers_of_10[prec])
		{
			*frac = 0;
			(*whole)++;
		}
	}
}

static inline char	*fast_dtoa_print(uint32_t whole, uint32_t frac, char *buff,
					const int_fast16_t prec)
{
	int_fast16_t	count;
	register char	*ptr;

	count = prec;
	ptr = buff;
	while (count-- > 0)
	{
		*ptr++ = (char)('0' + (frac % 10));
		frac /= 10;
	}
	if (frac > 0)
		++whole;
	if (prec > 0)
		*ptr++ = '.';
	*ptr++ = (char)(48 + (whole % 10));
	while (whole /= 10)
		*ptr++ = (char)(48 + (whole % 10));
	*ptr = 0;
	return (ptr);
}

static inline size_t	ft_fast_dtoa(double val, char *buff,
						const int_fast16_t prec)
{
	static const double	powers_of_10[] = {1, 10, 100, 1000, 10000, 100000,
	1000000, 10000000, 100000000, 1000000000};
	char				*ptr;
	uint32_t			whole;
	uint32_t			frac;

	if (val < 0.0 || val != val)
		return (0);
	whole = (uint32_t)val;
	val = (val - whole) * powers_of_10[prec];
	frac = (uint32_t)(val);
	fast_dtoa_roundup(val - frac, prec, &whole, &frac);
	ptr = fast_dtoa_print(whole, frac, buff, prec);
	ft_strrev(buff, ptr - 1);
	return ((size_t)(ptr - buff));
}

void					fast_dtoa(t_printf_info *info, double val)
{
	char		buff[16];
	const char	sign = (char)(val < 0.0);
	size_t		to_print;

	to_print = ft_fast_dtoa(val, buff, info->prec);
	info->width -= set_prefix_fp(info, sign, to_print);
	do_print(info, buff, to_print);
	padding(info, info->width, ' ');
}
