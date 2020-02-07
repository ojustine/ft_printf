#include "ft_printf.h"

static inline void	fxd_dbl_add(t_fxd *res, t_fxd *a)
{
	register int_fast16_t	i;
	register int_fast32_t	carry;

	res->frc_len = (res->frc_len > a->frc_len) ? res->frc_len
											   : a->frc_len;
	res->int_len = (res->int_len > a->int_len) ? res->int_len
											   : a->int_len;
	i = res->frc_len;
	carry = 0;
	while (--i >= -res->int_len)
	{
		if (a->val[D_F0 + i] == 0 && carry == 0)
			continue ;
		res->val[D_F0 + i] += (a->val[D_F0 + i] + carry);
		carry = res->val[D_F0 + i] / FP_R_LIMITER;
		res->val[D_F0 + i] %= FP_R_LIMITER;
	}
	res->val[D_F0 + i] = carry;
	while (res->val[D_F0 + i] == 0 && i < 0)
		i++;
	res->int_len = -i;
}

void				fxd_dbl_mul(t_fxd *res, t_fxd *a, t_fxd *b)
{
	register uint64_t		rank;
	register int_fast16_t	i;
	register int_fast16_t	j;
	t_fxd					*line;
	t_fxd					*tmp;

	line = fxd_new(a->frc_len + b->frc_len, 0);
	tmp = fxd_new(a->frc_len + b->frc_len, 0);
	i = b->frc_len;
	while (--i >= -b->int_len && (j = a->frc_len) < D_LEN)
		while (--j >= -a->int_len)
		{
			if ((rank = (uint64_t)a->val[D_F0 + j] * b->val[D_F0 + i]) == 0)
				continue ;
			line->val[D_F0 + i + j + 1] += rank % FP_R_LIMITER;
			line->val[D_F0 + i + j] += rank / FP_R_LIMITER;
			line->frc_len = (i + j + 1 >= 0) ? (i + j + 2) : 0;
			line->int_len = 0;
			if (i + j < 0)
				line->int_len = (i + j == -1) ? 1 : -(i + j);// + 1);
			fxd_dbl_add(tmp, line);
			ft_bzero(&(line->val[D_F0 + i + j]), FP_R_SIZE * 2);
		}
	ft_memswap(res, tmp, sizeof(t_fxd));
	fxd_del(tmp, line, 0);
}

void				fxd_dbl_build_mantis(t_binary64 bin64, t_fxd *res)
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
				res->val[D_F0 + j] += (g_pow_2_n[i][j] + carry);
				carry = res->val[D_F0 + j] / FP_R_LIMITER;
				res->val[D_F0 + j] %= FP_R_LIMITER;
				j--;
			}
		bin64.s_parts.mantis >>= 1;
		i--;
	}
	res->frc_len = 8;
	res->val[D_I0] = (bin64.s_parts.bias_exp != 0);
	res->int_len = res->val[D_I0];
}

static inline void	fxd_dbl_build_frac_exp(int_fast16_t b_exp, t_fxd *exp,
											 t_fxd *mul)
{
	if (b_exp < -64)
	{
		ft_memcpy(&mul->val[D_F0], g_pow_2_n[63], FP_R_SIZE * 8);
		mul->frc_len = 8;
		while (b_exp < -64)
		{
			fxd_dbl_mul(exp, exp, mul);
			b_exp += 64;
		}
	}
	if (b_exp >= -64 && b_exp < 0)
	{
		ft_memcpy(&mul->val[D_F0], g_pow_2_n[-b_exp - 1], FP_R_SIZE * 8);
		if (b_exp >= -18)
			mul->frc_len = (b_exp >= -9) ? 1 : 2;
		else if (b_exp >= -36)
			mul->frc_len = (b_exp >= -27) ? 3 : 4;
		else if (b_exp >= -54)
			mul->frc_len = (b_exp >= -45) ? 5 : 6;
		else
			mul->frc_len = (b_exp >= -63) ? 7 : 8;
		fxd_dbl_mul(exp, exp, mul);
	}
	fxd_del(mul, 0, 0);
}

void				fxd_dbl_build_exp(int_fast16_t b_exp, t_fxd *exp)
{
	t_fxd		*mul;

	exp->val[D_I0] = 1;
	exp->int_len = 1;
	b_exp -= 1023;
	if (b_exp == 0)
		return ;
	mul = fxd_new(8, 0);
	if (b_exp > 64)
	{
		ft_memcpy(&mul->val[D_I0 - 2], g_pow_2[63], FP_R_SIZE * 3);
		mul->int_len = 3;
		while (b_exp > 64 && (b_exp -= 64))
			fxd_dbl_mul(exp, exp, mul);
	}
	if (b_exp <= 64 && b_exp > 0)
	{
		ft_memcpy(&mul->val[D_I0 - 2], g_pow_2[b_exp - 1], FP_R_SIZE * 3);
		mul->int_len = (b_exp < 30) ? 1 : 2;
		mul->int_len += (b_exp < 60) ? 0 : 1;
		fxd_dbl_mul(exp, exp, mul);
	}

	fxd_dbl_build_frac_exp(b_exp, exp, mul);
}

void				fxd_build_exp(int_fast16_t b_exp, t_fxd *exp)
{
	t_fxd			*mul;
	uint32_t		**table;
	int_fast16_t	less_zero;

	exp->val[D_I0] = 1;
	exp->int_len = 1;
	b_exp = ft_abs(b_exp - 1023);
	less_zero = (b_exp < 0);
	table = (uint32_t**)((less_zero) ? g_pow_2_n : g_pow_2);
	mul = fxd_new(8, 0);
	if (b_exp == 0)
		return ;
	while (b_exp > 64)
	{

	}
	if (b_exp <= 64 && b_exp != 0)
	{
		ft_memcpy(&mul->val[D_F0 - 3], table[b_exp], FP_R_SIZE * 8);
	}
}
