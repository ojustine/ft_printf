#include "ft_printf.h"
#define RANK_LIMITER 1000000000

void				fxd_dbl_add(t_fxd_dbl *res, t_fxd_dbl *term)
{
	register int_fast16_t	i;
	int_fast16_t			int_len;
	int_fast32_t			carry;

	i = (res->frac_len > term->frac_len) ? res->frac_len : term->frac_len;
	res->frac_len = i;
	carry = 0;
	while (--i >= 0)
	{
		res->frac[i] += (term->frac[i] + carry);
		carry = res->frac[i] / RANK_LIMITER;
		res->frac[i] %= RANK_LIMITER;
	}
	i = 0;
	int_len = (res->int_len > term->int_len) ? res->int_len : term->int_len;
	while (i < int_len)
	{
		res->ints[i] += (term->ints[i] + carry);
		carry = res->ints[i] / RANK_LIMITER;
		res->ints[i] %= RANK_LIMITER;
		i++;
	}
	res->ints[i] = carry;
	res->int_len = (carry) ? (i + 1) : i;
}

void				fxd_dbl_mult_by_num(t_fxd_dbl *res, t_fxd_dbl *line,
					int_fast32_t num, int_fast16_t offset)
{
	register int_fast32_t	rank;
	register int_fast32_t	tens;
	register int_fast16_t	i;

	i = res->frac_len;
	while (--i >= 0)
	{
		rank = res->frac[i];
		tens = RANK_LIMITER;
		while (rank && ((tens /= 10) > 0))
			line->frac[i - offset + 1] += (ft_moddiv(res->frac[-offset] * num, tens,
			(intmax_t*)&line->frac[i - offset]) * tens);
//			line->frac[i - offset] = res->frac[-offset] * num;
//			line->frac[i - offset + 1] = line->frac[i - offset] % tens;
//			line->frac[i - offset] /= tens;
	}
	i = 0;
	while (++i < res->int_len)
	{
		rank = res->ints[i];
		tens = 1;
		while (rank && ((tens *= 10) > RANK_LIMITER))
			line->ints[i + offset + 1] += (ft_moddiv(res->frac[-offset] * num, tens,
													 (intmax_t*)&line->frac[i + offset]) * tens);
	}
}

void				fxd_dbl_mult(t_fxd_dbl *res, t_fxd_dbl *mult)
{
	register int_fast16_t	i;
	t_fxd_dbl				tmp;
	t_fxd_dbl				line;
	register int_fast32_t	tens;

	ft_memset(&tmp, 0, sizeof(t_fxd_dbl));
	i = mult->frac_len + 1;
	while (--i > 0)
	{
		tens = RANK_LIMITER;
		while (mult->frac[i - 1] != 0 && ((tens /= 10) > 0))
		{
			ft_memset(&line, 0, sizeof(t_fxd_dbl));
			fxd_dbl_mult_by_num(res, &line, ft_moddiv(mult->frac[i - 1], 10, (intmax_t*)&mult->frac[i - 1]), -i);
			fxd_dbl_add(&tmp, &line);
		}
	}
	while (++i <= mult->int_len)
	{
		tens = 1;
		while (mult->ints[i] != 0 && ((tens *= 10) < RANK_LIMITER))
		{
			fxd_dbl_mult_by_num(res, (mult->ints[i] % 10), i, tens);
			mult->ints[i] /= 10;
		}
	}
}

void				fxd_dbl_pow(t_fxd_dbl *res, int_fast16_t pow)
{
//	t_fxd_dbl multiplier;
//
//
//	if (pow == 0)
//		return ;
//	if (pow < 64 && pow > -64)
//		fxd_dbl_mult();
//	else
//		return ;
}

void				fxd_dbl_build_mantis(t_binary64 bin64, t_fxd_dbl *fxd_dbl)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	int_fast32_t			carry;

	i = 51;
	while (bin64.s_parts.mantis != 0)
	{
		j = 7;
		carry = 0;
		if (bin64.s_parts.mantis & 1U)
			while (j >= 0)
			{
				fxd_dbl->frac[j] += (g_powers_2_neg[i][j] + carry);
				carry = fxd_dbl->frac[j] / RANK_LIMITER;
				fxd_dbl->frac[j] %= RANK_LIMITER;
				j--;
			}
		bin64.s_parts.mantis >>= 1U;
		i--;
	}
	fxd_dbl->frac_len = 8;
	fxd_dbl->ints[0] = (bin64.s_parts.bias_exp != 0);
	fxd_dbl->int_len = fxd_dbl->ints[0];
}

void				fxd_dbl_build_exp(t_binary64 bin64,
									  t_fxd_dbl *fxd_dbl)
{
	register int_fast16_t	i;
	t_fxd_dbl		exp;

	ft_memset(&exp, 0, sizeof(t_fxd_dbl));
	bin64.s_parts.bias_exp -= 1023;
}
