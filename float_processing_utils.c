#include "ft_printf.h"

static inline void	big_float_multiply_line(t_big_float *a, t_big_float *line,
							const int_fast16_t mult, const int_fast16_t prec)
{
	register ssize_t		i;
	register int_fast32_t	result;
	register int_fast32_t	carry;

	i = prec - 1;
	carry = 0;
	while (i >= 0)
	{
		result = carry;
		result += a->digits[i] * mult;
		carry = result / 10;
		line->digits[i] = (char)(result % 10);
		i--;
	}
}

static inline void	big_float_align_each_other(t_big_float *a, t_big_float *b,
					const int_fast16_t prec)
{
	if (b->point_pos > a->point_pos)
	{
		big_float_shift_right(a, prec, b->point_pos - a->point_pos);
		a->point_pos = b->point_pos;
	}
	else if (b->point_pos < a->point_pos)
	{
		big_float_shift_right(b, prec, a->point_pos - b->point_pos);
		b->point_pos = a->point_pos;
	}
}

void				big_float_multiply(t_big_float *a, t_big_float *b,
					t_big_float *res, int_fast16_t prec)
{
	register ssize_t		i;
	t_big_float				line;
	t_big_float				temp;
prec += a->point_pos + b->point_pos - 1;
	big_float_parse(&line, "0.0", 1);
	big_float_parse(&temp, "0.0", 1);
	ft_memset(res, 0, sizeof(t_big_float));
	res->point_pos = prec;
	line.point_pos = prec;
	big_float_move_value(a, prec, 1);
	big_float_move_value(b, prec, 1);
	i = prec - 1;
	while (i >= 0)
	{
		big_float_multiply_line(a, &line, b->digits[i], prec);
		big_float_shift_left(&line, prec, prec - i);
		big_float_add(res, &line, &temp, prec);
		line.point_pos = prec;
		big_float_move_value(&temp, prec, 1);
		ft_memcpy(res, &temp, sizeof(t_big_float));
		i--;
	}
	res->point_pos -= prec - a->point_pos + prec - b->point_pos + 1;
	big_float_move_value(res, prec, 0);
	res->sign = ((a->sign || b->sign) && !(a->sign && b->sign)) ? 1 : 0;
}

void				big_float_add(t_big_float *a, t_big_float *b,
					t_big_float *res, const int_fast16_t prec)
{
	register ssize_t		i;
	register int_fast32_t	result;
	register int_fast32_t	carry;

	carry = 0;
	big_float_align_each_other(a, b, prec);
	ft_memset(res, 0, sizeof(t_big_float));
	res->point_pos = a->point_pos;
	i = prec - 1;
	while (i >= 0)
	{
		result = carry;
		result += a->digits[i] + b->digits[i];
		carry = result / 10;
		res->digits[i] = (char)(result % 10);
		i--;
	}
	if (carry != 0)
	{
		big_float_shift_right(res, prec, 1);
		res->point_pos++;
		res->digits[0] = carry;
	}
	big_float_move_value(res, prec, 0);
}
