#include "ft_printf.h"

void	bf_parse(t_big_float *f, const char *str)
{
	register size_t	i;

	i = 0;
	ft_memset(f, 0, sizeof(t_big_float));
	if (*str == '-' || *str == '+')
	{
		f->sign = (*str == '-');
		str++;
	}
	while (i < FLT_MAX_LEN)
	{
		if (*str >= '0' && *str <= '9')
			f->digits[i++] = (char)(*str - '0');
		else if ((*str == '.' || *str == ',') && !f->point_pos)
			f->point_pos = ((i == 0) ? 1 : i);
		else if (*str == ' ' || *str == '\t')
			continue ;
		else
			break ;
		str++;
	}
	f->length = i;
}

void	bf_shift_right(t_big_float *a, const int_fast16_t shift)
{
	register ssize_t	i;

	if (shift < 0)
	{
		bf_shift_left(a, -shift);
		return ;
	}
	a->length += shift;
	if (a->length > FLT_MAX_LEN)
		a->length = FLT_MAX_LEN;
	i = a->length;
	while (--i >= 0)
	{
		if (i - shift >= 0)
			a->digits[i] = a->digits[i - shift];
		else
			a->digits[i] = 0;
	}
}

void	bf_shift_left(t_big_float *a, const int_fast16_t shift)
{
	register ssize_t	i;
	int_fast16_t		stop;

	if (shift < 0)
	{
		bf_shift_right(a, -shift);
		return ;
	}
	i = -1;
	a->length -= shift;
	while (++i + shift < FLT_MAX_LEN)
	{
		if (i + shift < length)
			ft_memmove(&a->digits[i], &a->digits[i + shift], 1);//a->digits[i] = a->digits[i + shift];
		else
			a->digits[i] = 0;
	}
}

void	bf_move_value(t_big_float *a, const int_fast16_t prec,
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
		bf_shift_right(a, prec - start - 1);
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
		bf_shift_left(a, prec, start);
		a->point_pos -= start;
	}
}
