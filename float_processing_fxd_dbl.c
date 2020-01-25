#include "ft_printf.h"
#define RANK_LIMITER 1000000000

void				fxd_dbl_add(t_fxd_dbl *base, t_fxd_dbl *term)
{
	register int_fast16_t	i;
	register int_fast32_t	carry;

	base->frc_len = (base->frc_len > term->frc_len) ? base->frc_len
													: term->frc_len;
	base->int_len = (base->int_len > term->int_len) ? base->int_len
													: term->int_len;
	i = base->frc_len;
	carry = 0;
	while (--i >= -base->int_len)
	{
		if (term->val[FRC_0 + i] == 0 && carry == 0)
			continue ;
		base->val[FRC_0 + i] += (term->val[FRC_0 + i] + carry);
		carry = base->val[FRC_0 + i] / RANK_LIMITER;
		base->val[FRC_0 + i] %= RANK_LIMITER;
	}
	base->val[FRC_0 + i] = carry;
	base->int_len = (carry) ? (-i) : base->int_len;
}

void				fxd_dbl_mul(t_fxd_dbl *base, t_fxd_dbl *mul)
{
	register uint64_t		rank_res;
	register int_fast16_t	i;
	register int_fast16_t	j;
	t_fxd_dbl				tmp;
	t_fxd_dbl				line;

	ft_memset(&tmp, 0, sizeof(t_fxd_dbl));
	i = mul->frc_len;
	while (--i >= -mul->int_len)
	{
		j = base->frc_len;
		while (--j >= -base->int_len)
		{
			if ((rank_res = base->val[FRC_0 + j] * mul->val[FRC_0 + i]) == 0)
				continue ;
			ft_memset(&line, 0, sizeof(t_fxd_dbl));
			line.val[FRC_0 + i + j + 1] = rank_res % RANK_LIMITER;
			line.val[FRC_0 + i + j] = rank_res / RANK_LIMITER;
			line.frc_len = (i + j + 1 >= 0) ? (i + j + 2) : 0;
			line.int_len = (i + j < 0) ? -(i + j + 1) : 0;
			fxd_dbl_add(&tmp, &line);
		}
	}
	ft_memcpy(base, &tmp, sizeof(t_fxd_dbl));
}

void				fxd_dbl_build_mantis(t_binary64 bin64, t_fxd_dbl *mantis)
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
				mantis->val[FRC_0 + j] += (g_pow_2_n[i][j] + carry);
				carry = mantis->val[FRC_0 + j] / RANK_LIMITER;
				mantis->val[FRC_0 + j] %= RANK_LIMITER;
				j--;
			}
		bin64.s_parts.mantis >>= 1U;
		i--;
	}
	mantis->frc_len = 8;
	mantis->val[INT_0] = (bin64.s_parts.bias_exp != 0);
	mantis->int_len = mantis->val[INT_0];
}

static inline void	fxd_dbl_build_frac_exp(int_fast16_t exp, t_fxd_dbl *base,
					t_fxd_dbl *mul)
{
	if (exp == 0)
		return ;
	if (exp < -64)
	{
		ft_memcpy(&mul->val[FRC_0], g_pow_2_n[63], 8);
		mul->frc_len = 8;
		while (exp < -64)
		{
			fxd_dbl_mul(base, mul);
			exp += 64;
		}
	}
	if (exp >= -64)
	{
		ft_memcpy(&mul->val[FRC_0], g_pow_2_n[-exp - 1], 8);
		if (exp >= -18)
			mul->frc_len = (exp >= -9) ? 1 : 2;
		else if (exp >= -36)
			mul->frc_len = (exp >= -27) ? 3 : 4;
		else if (exp >= -54)
			mul->frc_len = (exp >= -45) ? 5 : 6;
		else
			mul->frc_len = (exp >= -63) ? 7 : 8;
		fxd_dbl_mul(base, mul);
	}
}

void				ffxd_dbl_build_exp(int_fast16_t exp, t_fxd_dbl *base,
					t_fxd_dbl *mul)
{
	if (exp > 0)
	{
		if (exp <= 64)
		{
			ft_memcpy(&base->val[INT_0 - 2], g_pow_2[exp - 1], 3);
			base->int_len = (exp < 30) ? 1 : 2;
			base->int_len += (base->val[INT_0 - 2] != 0);
			return ;
		}
		ft_memcpy(&base->val[INT_0 - 2], g_pow_2[63], 3);
		base->int_len = 3;
		mul->int_len = 3;
		if (exp > 32)
		{
			ft_memcpy(&mul->val[INT_0 - 2], g_pow_2[31], 3);
			while (exp > 32 && (exp -= 32))
				fxd_dbl_mul(base, mul);
		}
	}
	else
		fxd_dbl_build_frac_exp(exp, base, mul);
}

void				fxd_dbl_build_exp(int_fast16_t b_exp, t_fxd_dbl *base)
{
	t_fxd_dbl mul;

	base->val[INT_0] = 1;
	base->int_len = 1;
	b_exp -= 1023;
	ft_memset(&mul, 0, sizeof(t_fxd_dbl));
	if (b_exp > 0)
	{
		if (b_exp > 64)
		{
			ft_memcpy(&mul.val[INT_0 - 2], g_pow_2[63], 3);
			mul.int_len = 3;
			while (b_exp > 64 && (b_exp -= 64))
				fxd_dbl_mul(base, &mul);
		}
		if (b_exp <= 64)
		{
			ft_memcpy(&mul.val[INT_0 - 2], g_pow_2[b_exp - 1], 3);
			mul.int_len = (b_exp < 30) ? 1 : 2;
			mul.int_len += (mul.val[INT_0 - 2] != 0);
			fxd_dbl_mul(base, &mul);
		}
	}
	else
		fxd_dbl_build_frac_exp(b_exp, base, &mul);
}
