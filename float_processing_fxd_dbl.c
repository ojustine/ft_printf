/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_processing_fxd_dbl.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 13:54:52 by slynell           #+#    #+#             */
/*   Updated: 2020/02/19 13:55:37 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static inline void			fxd_add(t_fxd *res, t_fxd *a)
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
		if (a->val[a->f0 + i] == 0 && carry == 0)
			continue ;
		res->val[res->f0 + i] += (a->val[a->f0 + i] + carry);
		carry = res->val[res->f0 + i] / FP_R_LIMITER;
		res->val[res->f0 + i] %= FP_R_LIMITER;
	}
	res->val[res->f0 + i] = carry;
	while (res->val[res->f0 + i] == 0 && i < 0)
		i++;
	res->int_len = -i;
}

void						fxd_dbl_mul(t_fxd *res, t_fxd *a, t_fxd *b,
							int_fast16_t is_long_dbl)
{
	register uint64_t		rank;
	register int_fast16_t	i;
	register int_fast16_t	j;
	t_fxd					*line;
	t_fxd					*tmp;

	line = fxd_new(a->frc_len + b->frc_len + 1, is_long_dbl);
	tmp = fxd_new(a->frc_len + b->frc_len + 1, is_long_dbl);
	i = b->frc_len;
	while (--i >= -b->int_len && (j = a->frc_len) | 1)
		while (--j >= -a->int_len)
		{
			if ((rank = (uint64_t)a->val[a->f0 + j] * b->val[b->f0 + i]) == 0)
				continue ;
			line->val[line->f0 + i + j + 1] += rank % FP_R_LIMITER;
			line->val[line->f0 + i + j] += rank / FP_R_LIMITER;
			line->frc_len = (i + j + 1 >= 0) ? (i + j + 2) : 0;
			line->int_len = 0;
			if (i + j < 0)
				line->int_len = (i + j == -1) ? 1 : -(i + j);
			fxd_add(tmp, line);
			ft_bzero(&(line->val[line->f0 + i + j]), (FP_R_SIZE) * 2);
		}
	ft_memswap(res, tmp, sizeof(t_fxd));
	fxd_del(tmp, line);
}

t_fxd						*fxd_build_mantis(uint64_t bin_mantis,
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
			ft_memcpy(&term->val[term->f0], g_pow_2_n[i], (FP_R_SIZE) * 8);
			term->frc_len = g_pow_2_n[i][8];
			fxd_add(mantis, term);
		}
		bin_mantis >>= 1;
		i--;
	}
	if (is_normal)
	{
		mantis->val[mantis->f0 - 1] = 1;
		mantis->int_len = 1;
	}
	fxd_del(term, 0);
	return (mantis);
}

t_fxd						*fxd_get_pow_2(register int_fast16_t pow,
							int_fast16_t is_long_dbl)
{
	t_fxd					*res;
	t_fxd					*base;

	res = fxd_new(0, is_long_dbl);
	res->val[res->f0 - 1] = 1;
	res->int_len = 1;
	base = fxd_new(1, is_long_dbl);
	base->val[base->f0 - 1] = (pow < 0) ? 0 : 2;
	base->int_len = (pow < 0) ? 0 : 1;
	base->val[base->f0] = (pow < 0) ? 500000000 : 0;
	base->frc_len = (pow < 0) ? 1 : 0;
	while (pow != 0)
		if (pow & 1)
		{
			fxd_dbl_mul(res, res, base, is_long_dbl);
			pow = (pow < 0) ? pow + 1 : pow - 1;
		}
		else
		{
			fxd_dbl_mul(base, base, base, is_long_dbl);
			pow /= 2;
		}
	fxd_del(base, 0);
	return (res);
}

void						fxd_roundup(t_fxd *fp, const int_fast32_t prec)
{
	register const uint64_t	pow = ft_pow(10, (FP_R_LEN - (prec % FP_R_LEN)));
	register uint_fast32_t	carry;
	register int_fast16_t	i;
	register int_fast16_t	j;

	i = prec / FP_R_LEN;
	j = i;
	if ((fp->val[fp->f0 + i] % pow) > (pow / 10 * 5))
		fp->val[fp->f0 + i] += pow;
	else if ((fp->val[fp->f0 + i] % pow) == (pow / 10 * 5))
	{
		while (++j < fp->frc_len && fp->val[fp->f0 + j] == 0)
			;
		if (j == fp->frc_len)
			fp->val[fp->f0 + i] += fp->val[fp->f0 + i] / pow % 10 & 1 ? pow : 0;
		else
			fp->val[fp->f0 + i] += pow;
	}
	while ((carry = fp->val[fp->f0 + i] / FP_R_LIMITER) > 0)
	{
		fp->val[fp->f0 + i] %= FP_R_LIMITER;
		fp->val[fp->f0 + --i] += carry;
		fp->int_len = (i < 0) ? -i : fp->int_len;
	}
}
