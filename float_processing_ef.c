/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_processing_ef.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 12:03:01 by slynell           #+#    #+#             */
/*   Updated: 2020/02/19 14:12:33 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void					fxd_ftoa_inf_nan(t_ptf_info *info,
						const uint64_t mantis, const char sign)
{
	const char	*inf_nan = (mantis != 0) ? "nanNAN" : "infINF";

	info->flags &= ~FLAG_ZERO_PAD;
	info->width -= set_prefix_fp(info, sign, 3);
	do_print(info, (char*)inf_nan + info->cap * 3, 3);
	padding(info, info->width, ' ');
}

static inline size_t	fxd_ftoa_dec_form_frac_part(t_ptf_info *info,
						t_fxd *fp, char *buff)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	register int_fast32_t	prec;
	const char				*ptr = buff;

	if (info->prec == 0 && !(info->flags & FLAG_ALT_FORM))
		return (0);
	*buff++ = '.';
	i = -1;
	prec = 0;
	while (++i < fp->frc_len)
	{
		j = FP_R_LEN;
		while ((prec < info->prec) && --j >= 0)
		{
			prec = i * FP_R_LEN + FP_R_LEN - j;
			*buff++ = (char)(fp->val[fp->f0 + i] / FP_R_TOP + '0');
			fp->val[fp->f0 + i] %= FP_R_TOP;
			fp->val[fp->f0 + i] *= 10;
		}
	}
	while (prec++ < info->prec)
		*buff++ = '0';
	return (buff - ptr);
}

size_t					fxd_ftoa_dec_form(t_ptf_info *info, t_fxd *fp,
						char *buff)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	const char				*ptr = buff;

	i = -fp->int_len;
	fxd_roundup(fp, info->prec);
	if (i == 0)
		*buff++ = '0';
	else
	{
		j = ft_intlen(fp->val[fp->f0 + i]);
		fp->val[fp->f0 + i] *= ft_pow(10, FP_R_LEN - j);
		while (i < 0)
		{
			while (--j >= 0)
			{
				*buff++ = (char)(fp->val[fp->f0 + i] / FP_R_TOP + '0');
				fp->val[fp->f0 + i] %= FP_R_TOP;
				fp->val[fp->f0 + i] *= 10;
			}
			j = FP_R_LEN;
			i++;
		}
	}
	return ((buff - ptr) + fxd_ftoa_dec_form_frac_part(info, fp, buff));
}

static inline int32_t	fxd_ftoa_normalize(t_ptf_info *info, t_fxd *fp,
						const int_fast16_t inx, register int_fast32_t offset)
{
	t_fxd					*mul;
	int_fast16_t			i;
	const uint64_t			pow = ft_pow(10, (FP_R_LEN - (info->prec % FP_R_LEN)));

	mul = fxd_new((inx < 0) ? -inx : 0, fp->f0 == FP_LD_POINT + 1);
	mul->frc_len = (inx < 0) ? -inx : 0;
	mul->int_len = (inx >= 0) ? inx + 1 : 0;
	mul->val[mul->f0 - 1 - inx] = ft_pow(10, FP_R_LEN - offset);
	fxd_dbl_mul(fp, fp, mul, fp->f0 == FP_LD_POINT + 1);
	fxd_roundup(fp, info->prec);
	if (fp->val[fp->f0 - 1] > FP_R_LEN)
	{
		mul->val[mul->f0] = FP_R_TOP;
		mul->int_len = 0;
		mul->frc_len = 1;
		fxd_dbl_mul(fp, fp, mul, fp->f0 == FP_LD_POINT + 1);
		offset++;
	}
	i = info->prec / FP_R_LEN;
	fp->val[fp->f0 + i] -= fp->val[fp->f0 + i] % pow;
	fxd_del(mul, 0);
	return (offset);
}

size_t					fxd_ftoa_exp_form(t_ptf_info *info, t_fxd *fp,
						char *buff)
{
	int_fast32_t			offset;
	int_fast16_t			inx;
	const char				*ptr = buff;

	inx = -fp->int_len;
	if (fp->int_len == 0 && fp->frc_len > 0)
		while (fp->val[fp->f0 + inx] == 0 && inx < fp->frc_len)
			inx++;
	offset = ft_intlen(fp->val[fp->f0 + inx]) - 1;
	offset = fxd_ftoa_normalize(info, fp, inx, offset);
	buff += fxd_ftoa_dec_form(info, fp, buff);
	*buff++ = "eE"[info->cap];
	*buff++ = "-+"[inx < 0 || (fp->int_len == 0 && fp->frc_len == 0)];
	if (inx >= 1)
		offset = FP_R_LEN - offset;
	if (fp->int_len == 0 && fp->frc_len == 0)
		offset = 0;
	else
		offset = ft_abs(((inx > 0) ? inx : -(inx + 1)) * FP_R_LEN + offset);
	if (offset < 10)
		*buff++ = '0';
	inx = ft_intlen(offset);
	while (inx-- > 0)
		*buff++ = (char)(ft_divmod(offset, ft_pow(10, inx), &offset) + '0');
	return (buff - ptr);
}
