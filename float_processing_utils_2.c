#include "ft_printf.h"

void	bf_parse(t_big_float *f, const char *str)
{
	register size_t	i;

	ft_memset(f, 0, sizeof(t_big_float));
	if ((*str == '-' || *str == '+') && str++)
		f->sign = (*str == '-');
	while (*str == ' ' || *str == '\t')
		str++;
	i = 0;
	while (*str && i < FLT_MAX_LEN)
	{
		if (*str >= '0' && *str <= '9')
			f->digits[i++] = (char)(*str - '0');
		else if ((*str == '.' || *str == ',') && (f->point_pos == 0))
		{
			if (i == 0)
				f->digits[i++] = 0;
			f->point_pos = i;
		}
		else
			break ;
		str++;
	}
	f->length = i;
	if (f->point_pos == 0)
		f->point_pos = f->length;
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
	i = a->length - 1;
	while (i >= 0)
	{
		if (i - shift >= 0)
			a->digits[i] = a->digits[i - shift];
		else
			a->digits[i] = 0;
		i--;
	}
}

void	bf_shift_left(t_big_float *a, const int_fast16_t shift)
{
	register ssize_t	i;

	if (shift < 0)
	{
		bf_shift_right(a, -shift);
		return ;
	}
	i = 0;
	if (a->length <= shift)
	{
		bf_parse(a, "0.0");
		return ;
	}
	while ((i + shift <= a->length) && (a->length < FLT_MAX_LEN))
	{
		a->digits[i] = a->digits[i + shift];
		i++;
	}
	a->length -= shift;
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
		bf_shift_left(a, start);
		a->point_pos -= start;
	}
}
