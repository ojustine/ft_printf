/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_processing_ag.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 12:02:17 by slynell           #+#    #+#             */
/*   Updated: 2020/02/17 12:02:40 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static inline size_t	fxd_ftoa_opt_form_f(t_printf_info *info,
							t_fxd *fp, char *buff)
{
	size_t		to_print;

	to_print = fxd_ftoa_dec_form(info, fp, buff);
	if (!(info->flags & FLAG_ALT_FORM))
	{
		buff += to_print;
		while (*buff-- == '0' && to_print > 0)
			to_print--;
		if (*buff == '.')
			to_print--;
	}
	return (to_print);
}

static inline size_t	fxd_ftoa_opt_form_e(t_printf_info *info,
							t_fxd *fp, char *buff)
{
	size_t			to_print;
	int_fast16_t	e_len;
	int_fast16_t	to_remove;
	char			*e_ptr;

	to_print = fxd_ftoa_exp_form(info, fp, buff);
	if (!(info->flags & FLAG_ALT_FORM))
	{
		buff += to_print;
		e_len = 1;
		to_remove = 0;
		while (*(buff - e_len) != 'e' && *(buff - e_len) != 'E')
			e_len++;
		buff -= e_len;
		e_ptr = buff;
		while (*(--buff) == '0' || *buff == '.')
			to_remove++;
		buff++;
		if (to_remove)
			ft_memcpy(buff, e_ptr, e_len);
		to_print -= to_remove;
	}
	return (to_print);
}

size_t					fxd_ftoa_opt_form(t_printf_info *info,
							t_fxd *fp, char *buff)
{
	int_fast32_t	offset;
	int_fast32_t	prec;
	int_fast16_t	inx;

	inx = fp->int_len;
	if (fp->int_len == 0 && fp->frc_len > 0)
		while (fp->val[fp->f0 - inx] == 0 && inx < fp->frc_len)
			inx--;
	offset = ft_intlen(fp->val[fp->f0 - inx]) - (inx > 0);
	if (inx < 1)
		offset = -(FP_R_LEN - offset);
	offset = ((inx > 0) ? (inx - 1) : inx) * FP_R_LEN + offset;
	prec = (info->prec) ? info->prec : 1;
	if (prec > offset && offset >= -4)
	{
		info->prec = prec - (offset + 1);
		return (fxd_ftoa_opt_form_f(info, fp, buff));
	}
	else
	{
		info->prec = prec - 1;
		return (fxd_ftoa_opt_form_e(info, fp, buff));
	}
}
