#include "ft_printf.h"

static inline void	big_float_multiply_line(t_big_float *a, t_big_float *line,
					const int_fast16_t multiplier, const int_fast16_t prec)
{
	register ssize_t		i;
	register int_fast32_t	result;
	register int_fast32_t	carry;

	i = prec - 1;
	carry = 0;
	while (i >= 0)
	{
		result = carry;
		result += a->digits[i] * multiplier;
		carry = result / 10;
		line->digits[i] = (char)(result % 10);
		i--;
	}
}

static inline void	big_float_align_each_other(t_big_float *a, t_big_float *b)
{
	if (b->point_pos > a->point_pos)
	{
		bf_shift_right(a, b->point_pos - a->point_pos);
		a->point_pos = b->point_pos;
	}
	else if (b->point_pos < a->point_pos)
	{
		bf_shift_right(b, a->point_pos - b->point_pos);
		b->point_pos = a->point_pos;
	}
}

void				bf_multiply(t_big_float *a, t_big_float *b,
								t_big_float *res, int_fast16_t prec)
{
	register ssize_t		i;
	t_big_float				line;
	t_big_float				temp;

	bf_parse(&line, "0.0");
	bf_parse(&temp, "0.0");
	ft_memset(res, 0, sizeof(t_big_float));
	res->point_pos = prec;
	line.point_pos = prec;
	bf_move_value(a, prec, 1);
	bf_move_value(b, prec, 1);
	i = prec - 1;
	while (i >= 0)
	{
		big_float_multiply_line(a, &line, b->digits[i], prec);
		bf_shift_left(&line, prec - i);
		bf_add(res, &line, &temp, prec);
		line.point_pos = prec;
		bf_move_value(&temp, prec, 1);
		ft_memcpy(res, &temp, sizeof(t_big_float));
		i--;
	}
	res->point_pos -= prec - a->point_pos + prec - b->point_pos + 1;
	bf_move_value(res, prec, 0);
	res->sign = ((a->sign || b->sign) && !(a->sign && b->sign)) ? 1 : 0;
}

void				bf_add(t_big_float *a, t_big_float *b, t_big_float *res,
					const int_fast16_t prec)
{
	register ssize_t		i;
	register int_fast32_t	result;
	register int_fast32_t	carry;

	carry = 0;
	big_float_align_each_other(a, b);
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
		bf_shift_right(res, 1);
		res->point_pos++;
		res->digits[0] = carry;
	}
	bf_move_value(res, prec, 0);
}
