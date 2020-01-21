#include "ft_printf.h"

void				fxd_dbl_add(t_fxd_dbl *res, t_fxd_dbl *term)
{
	register int_fast16_t	i;
	int_fast16_t			int_inx;
	int_fast32_t			carry;

	i = (res->frac_inx > term->frac_inx) ? res->frac_inx : term->frac_inx;
	res->frac_inx = i;
	carry = 0;
	while (i >= 0)
	{
		res->frac[i] += (term->frac[i] + carry);
		carry = res->frac[i] / 1000000000;
		res->frac[i] %= 1000000000;
		i--;
	}
	i = 0;
	int_inx = (res->int_inx > term->int_inx) ? res->int_inx : term->int_inx;
	while (i <= int_inx)
	{
		res->ints[i] += (term->ints[i] + carry);
		carry = res->ints[i] / 1000000000;
		res->ints[i] %= 1000000000;
		i++;
	}
	res->ints[i] = carry;
	res->int_inx = (carry) ? i : (i - 1);
}

void				fxd_dbl_mult_by_num(t_fxd_dbl *res, int_fast16_t offset,
										int_fast32_t num)
{

}

void				fxd_dbl_mult(t_fxd_dbl *res, t_fxd_dbl *mult)
{
	int_fast32_t	factor;
	register int_fast16_t	i;
	register int_fast32_t	tens;

	i = mult->frac_inx + 1;
	while (--i >= 0)
	{
		tens = 1;
		while (mult->frac[i] != 0)
		{
			factor = mult->frac[i] % 10;
			mult->frac[i] /= 10;
			fxd_dbl_mult_by_num(res, -i, factor * tens);
			tens *= 10;
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
				carry = fxd_dbl->frac[j] / 1000000000;
				fxd_dbl->frac[j] %= 1000000000;
				j--;
			}
		bin64.s_parts.mantis >>= 1U;
		i--;
	}
	fxd_dbl->frac_inx = 7;
	fxd_dbl->int_inx = 0;
	fxd_dbl->ints[0] += (bin64.s_parts.bias_exp != 0);
}

void				fxd_dbl_build_exp(t_binary64 bin64,
									  t_fxd_dbl *fxd_dbl)
{
	register int_fast16_t	i;
	t_fxd_dbl		exp;
	uint32_t		exp_ints[FXD_DBL_LEN];
	uint32_t		exp_frac[FXD_DBL_LEN];

	ft_memset(exp_ints, 0, sizeof(uint32_t) * FXD_DBL_LEN);
	ft_memset(exp_frac, 0, sizeof(uint32_t) * FXD_DBL_LEN);
	exp.ints = exp_ints;
	exp.frac = exp_frac;
	bin64.s_parts.bias_exp -= 1023;
}
