#include "ft_printf.h"

void	big_float_parse(t_big_float *f, const char *str)
{
	register size_t	j;

	j = 0;
	ft_memset(f, 0, sizeof(t_big_float));
	if (*str == '-' && *(str++))
		f->sign = 1;
	while (*str && j < FLT_MAX_LEN)
	{
		if (*str >= '0' && *str <= '9')
			f->digits[j++] = (char)(*str - '0');
		else if (*str == '.')
			f->point_pos = j;
		else
			break ;
		str++;
	}
	f->length = j;
}

void	big_float_shift_right(t_big_float *a, const int_fast16_t length,
		const int_fast16_t shift)
{
	register ssize_t	i;

	i = length;
	if (shift < 0)
	{
		big_float_shift_left(a, length, -shift);
		return ;
	}
	while (--i >= 0 && shift > 0)
	{
		if (i - shift >= 0)
			ft_memmove(&a->digits[i], &a->digits[i - shift], 1);//a->digits[i] = a->digits[i - shift];
		else
			a->digits[i] = 0;
	}
}

void	big_float_shift_left(t_big_float *a, const int_fast16_t length,
		const int_fast16_t shift)
{
	register ssize_t	i;

	i = -1;
	if (shift < 0)
	{
		big_float_shift_right(a, length, -shift);
		return ;
	}
	while (++i < length)
	{
		if (i + shift < length)
			ft_memmove(&a->digits[i], &a->digits[i + shift], 1);//a->digits[i] = a->digits[i + shift];
		else
			a->digits[i] = 0;
	}
}

void	big_float_move_value(t_big_float *a, const int_fast16_t prec,
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
