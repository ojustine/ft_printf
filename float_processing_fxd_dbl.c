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

void				fxd_dbl_build_mantis(t_fxd *res, uint64_t mantis,
					int_fast16_t is_normal, int_fast16_t is_long_dbl)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	int_fast32_t			carry;

	i = (is_long_dbl) ? 64 : 51;
	while (mantis != 0 && i >= 0)
	{
		j = 7;
		carry = 0;
		if (mantis & 1)
			while (j >= 0)
			{
				res->val[D_F0 + j] += (g_pow_2_n[i][j] + carry);
				carry = res->val[D_F0 + j] / FP_R_LIMITER;
				res->val[D_F0 + j] %= FP_R_LIMITER;
				j--;
			}
		mantis >>= 1;
		i--;
	}
	res->frc_len = 8;
	res->val[D_I0] = is_normal;
	res->int_len = is_normal;
}

t_fxd				*fxd_dbl_build_mantiss(uint64_t bin_mantis,
					int_fast16_t is_normal, int_fast16_t is_long_dbl)
{
	register int_fast16_t	i;
	t_fxd					*mantis;
	t_fxd					*term;

	mantis = fxd_new(8, is_long_dbl);
	term = fxd_new(8, is_long_dbl);
	i = (is_long_dbl) ? 63 : 51;
	while (bin_mantis != 0 && i >= 0)
	{
		if (bin_mantis & 1)
			{
				ft_memcpy(&term->val[D_F0], g_pow_2_n[i], FP_R_SIZE * 8);
				term->frc_len = g_pow_2_n[i][8];
				fxd_dbl_add(mantis, term);
			}
		bin_mantis >>= 1;
		i--;
	}
	if (is_normal)
	{
		mantis->val[D_I0] = 1;
		mantis->int_len = 1;
	}
	fxd_del(term, 0, 0);
	return (mantis);
}

t_fxd				*fxd_get_pow_2(int_fast16_t pow, int_fast16_t is_long_dbl)
{
	t_fxd			*res;
	t_fxd			*base;

	res = fxd_new(0, is_long_dbl);
	res->val[D_I0] = 1;
	res->int_len = 1;
	base = fxd_new(1, is_long_dbl);
	base->val[D_I0] = (pow < 0) ? 0 : 2;
	base->int_len = (pow < 0) ? 0 : 1;
	base->val[D_F0] = (pow < 0) ? 500000000 : 0;
	base->frc_len = (pow < 0) ? 1 : 0;
	while (pow != 0)
		if (pow & 1)
		{
			fxd_dbl_mul(res, res, base);
			pow = (pow < 0) ? pow + 1 : pow - 1;
		}
		else
		{
			fxd_dbl_mul(base, base, base);
			pow /= 2;
		}
	fxd_del(base, 0, 0);
	return (res);
}
