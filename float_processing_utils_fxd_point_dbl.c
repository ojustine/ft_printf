#include "ft_printf.h"
#define RANK_LIMITER 1000000000

void				fxd_dbl_add(t_fxd_dbl *res, t_fxd_dbl *trm)
{
	register int_fast16_t	i;
	register int_fast32_t	carry;

	res->frc_len = (res->frc_len > trm->frc_len) ? res->frc_len : trm->frc_len;
	res->int_len = (res->int_len > trm->int_len) ? res->int_len : trm->int_len;
	i = res->frc_len;
	carry = 0;
	while (--i >= -res->int_len)
	{
		res->val[FRC_0 + i] += (trm->val[FRC_0 + i] + carry);
		carry = res->val[FRC_0 + i] / RANK_LIMITER;
		res->val[FRC_0 + i] %= RANK_LIMITER;
	}
	res->val[FRC_0 + i] = carry;
	res->int_len = (carry) ? (-i) : res->int_len;
}

void				fxd_dbl_mult_by_num(t_fxd_dbl *line, uint_fast32_t rank,
					uint_fast32_t mult, int_fast16_t offset)
{
	//register int_fast32_t	rank;
	register int_fast32_t	tens;
	//register int_fast16_t	i;

	//i = res->frc_len;
	//while (--i >= -res->int_len)
	//{
		//rank = res->val[FRC_0 + i];
		tens = RANK_LIMITER;
		while (rank && ((tens /= 10) > 0))
		{
//			line->val[FRC_0 + i + offset] += (
//					ft_moddiv(res->val[FRC_0 + offset] * num, tens,
//							  (intmax_t *)&line->val[FRC_0 + i + offset]) *
//					tens);
			line->val[FRC_0 + offset] = rank * num;
			line->frac[i - offset + 1] = line->frac[i - offset] % tens;
			line->frac[i - offset] /= tens;
		}
	//}
}

void				fxd_dbl_mul(t_fxd_dbl *res, t_fxd_dbl *mul)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	t_fxd_dbl				tmp;
	t_fxd_dbl				line;

	ft_memset(&tmp, 0, sizeof(t_fxd_dbl));
	i = mul->frc_len;
	while (--i >= -mul->int_len)
	{
		j = res->frc_len;
		while (--j >= -res->int_len)
		{
			ft_memset(&line, 0, sizeof(t_fxd_dbl));
			fxd_dbl_mult_by_num(&line, res->val[FRC_0 + j], mul->val[FRC_0 + i],
					i + j + 1);
			fxd_dbl_add(&tmp, &line);
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
//		fxd_dbl_mul();
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
				fxd_dbl->val[FRC_0 + j] += (g_powers_2_neg[i][j] + carry);
				carry = fxd_dbl->val[FRC_0 + j] / RANK_LIMITER;
				fxd_dbl->val[FRC_0 + j] %= RANK_LIMITER;
				j--;
			}
		bin64.s_parts.mantis >>= 1U;
		i--;
	}
	fxd_dbl->frc_len = 8;
	fxd_dbl->val[INT_0] = (bin64.s_parts.bias_exp != 0);
	fxd_dbl->int_len = fxd_dbl->val[INT_0];
}

void				fxd_dbl_build_exp(t_binary64 bin64,
									  t_fxd_dbl *fxd_dbl)
{
	register int_fast16_t	i;
	t_fxd_dbl		exp;

	ft_memset(&exp, 0, sizeof(t_fxd_dbl));
	bin64.s_parts.bias_exp -= 1023;
}
