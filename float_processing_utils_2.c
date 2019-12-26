#include "ft_printf.h"

void	big_float_parse(t_big_float *bf, const char *str, int_fast16_t prec)
{
	register size_t	i;
	size_t			index;
	size_t			len;

	i = 0;
	ft_memset(bf, 0, sizeof(t_big_float));
	if (str[0] == '-')
	{
		bf->sign = 1;
		i = 1;
	}
	else
		bf->sign = 0;
	index = 0;
	len = ft_strlen(str);
	while (i < len && index < prec)
	{
		if (str[i] == '.')
			bf->point_pos = (bf->sign) ? i - 1 : i;
		else
			bf->digits[index++] = (char)(str[i] - '0');
		i++;
	}
}

void	big_float_shift_right(t_big_float *a, int_fast16_t length,
							  uint_fast16_t shift)
{
	register ssize_t	i;

	i = length;
	while (--i >= 0)
	{
		if (i - shift >= 0)
			a->digits[i] = a->digits[i - shift];
		else
			a->digits[i] = 0;
	}
}

void	big_float_shift_left(t_big_float *a, int_fast16_t length,
							 uint_fast16_t shift)
{
	register ssize_t	i;

	i = -1;
	while (++i < length)
	{
		if (i + shift < length)
			a->digits[i] = a->digits[i + shift];
		else
			a->digits[i] = 0;
	}
}

void	big_float_move_value(t_big_float *a, int_fast16_t prec,
								int_fast8_t is_move_to_tail)
{
	register ssize_t	i;
	int_fast16_t		start;

	if (is_move_to_tail)
	{
		i = prec - 1;
		while (i >= 0 && !a->digits[i])
			i--;
		start = i;
		big_float_shift_right(a, prec, prec - start - 1);
		a->point_pos += prec - start - 1;
	}
	else
	{
		i = 0;
		while (i < prec && !a->digits[i])
			i++;
		if (a->point_pos - i < 1)
			i = a->point_pos - 1;
		start = i;
		big_float_shift_left(a, prec, start);
		a->point_pos -= start;
	}
}
