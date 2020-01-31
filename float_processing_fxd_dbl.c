#include "ft_printf.h"

static inline void	fxd_dbl_add(t_fxd *base, t_fxd *term)
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
		if (term->val[D_F0 + i] == 0 && carry == 0)
			continue ;
		base->val[D_F0 + i] += (term->val[D_F0 + i] + carry);
		carry = base->val[D_F0 + i] / R_LIMITER;
		base->val[D_F0 + i] %= R_LIMITER;
	}
	base->val[D_F0 + i] = carry;
	base->int_len = (carry) ? (-i) : base->int_len;
}

void				fxd_dbl_mul(t_fxd *base, t_fxd *mul)
{
	register uint64_t		res;
	register int_fast16_t	i;
	register int_fast16_t	j;
	t_fxd_dbl_fast		line;
	t_fxd				*tmp;

	ft_bzero(&line, sizeof(t_fxd_dbl_fast));
	tmp = fxd_new(D_LEN, 0);
	i = mul->frc_len;
	while (--i >= -mul->int_len && (j = base->frc_len) < D_LEN)
		while (--j >= -base->int_len)
		{
			if ((res = (uint64_t)base->val[D_F0 + j] * mul->val[D_F0 + i]) == 0)
				continue ;
			line.val[D_F0 + i + j + 1] += res % R_LIMITER;
			line.val[D_F0 + i + j] += res / R_LIMITER;
			line.frc_len = (i + j + 1 >= 0) ? (i + j + 2) : 0;
			if (i + j < 0)
				line.int_len = (i + j == -1) ? 1 : -(i + j + 1);
			else
				line.int_len = 0;
			fxd_dbl_add(tmp, &line);
			ft_bzero(&line.val[D_F0 + i + j], R_SIZE * 2);
		}
	//ft_memcpy(base, tmp, sizeof(t_fxd_dbl_fast));
	fxd_del(base);
	base = tmp;
}

void				fxd_dbl_build_mantis(t_binary64 bin64, t_fxd *mantis)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	int_fast32_t			carry;

	i = 51;
	while (bin64.s_parts.mantis != 0)
	{
		j = 7;
		carry = 0;
		if (bin64.s_parts.mantis & 1)
			while (j >= 0)
			{
				mantis->val[D_F0 + j] += (g_pow_2_n[i][j] + carry);
				carry = mantis->val[D_F0 + j] / R_LIMITER;
				mantis->val[D_F0 + j] %= R_LIMITER;
				j--;
			}
		bin64.s_parts.mantis >>= 1;
		i--;
	}
	mantis->frc_len = 8;
	mantis->val[D_I0] = (bin64.s_parts.bias_exp != 0);
	mantis->int_len = mantis->val[D_I0];
}

static inline void	fxd_dbl_build_frac_exp(int_fast16_t exp, t_fxd *base,
											 t_fxd *mul)
{
	if (exp == 0)
		return ;
	if (exp < -64)
	{
		ft_memcpy(&mul->val[D_F0], g_pow_2_n[63], R_SIZE * 8);
		mul->frc_len = 8;
		while (exp < -64)
		{
			fxd_dbl_mul(base, mul);
			exp += 64;
		}
	}
	if (exp >= -64)
	{
		ft_memcpy(&mul->val[D_F0], g_pow_2_n[-exp - 1], R_SIZE * 8);
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

void				fxd_dbl_build_exp(int_fast16_t exp, t_fxd *base)
{
	t_fxd		mul;
	uint32_t	short_fxd_dbl[D_POINT + 8];

	base->val[D_I0] = 1;
	base->int_len = 1;
	exp -= 1023;
	ft_bzero(&mul, sizeof(t_fxd));
	ft_bzero(short_fxd_dbl, D_POINT + 8);
	mul.val = short_fxd_dbl;
	if (exp > 64)
	{
		ft_memcpy(&mul.val[D_I0 - 2], g_pow_2[63], R_SIZE * 3);
		mul.int_len += 3;
		while (exp > 64 && (exp -= 64))
			fxd_dbl_mul(base, &mul);
	}
	if (exp <= 64 && exp > 0)
	{
		ft_memcpy(&mul.val[D_I0 - 2], g_pow_2[exp - 1], R_SIZE * 3);
		mul.int_len += (exp < 30) ? 1 : 2;
		mul.int_len += (exp < 60) ? 0 : 1;
		fxd_dbl_mul(base, &mul);
	}
	else
		fxd_dbl_build_frac_exp(exp, base, &mul);
}
