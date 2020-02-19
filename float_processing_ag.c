/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_processing_ag.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 12:02:17 by slynell           #+#    #+#             */
/*   Updated: 2020/02/19 13:49:32 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static inline size_t	fxd_ftoa_opt_form_f(t_ptf_info *info,
						t_fxd *fp, char *buff)
{
	size_t				to_print;

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

static inline size_t	fxd_ftoa_opt_form_e(t_ptf_info *info,
						t_fxd *fp, char *buff)
{
	size_t				to_print;
	int_fast16_t		e_len;
	int_fast16_t		to_remove;
	char				*e_ptr;

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

size_t					fxd_ftoa_opt_form(t_ptf_info *info,
						t_fxd *fp, char *buff)
{
	int_fast32_t		offset;
	int_fast32_t		prec;
	int_fast16_t		inx;

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

static inline void		ldtoa_hex_form_roundup(t_ptf_info *info, char *buff)
{
	int_fast16_t		i;

	if (info->prec < 16)
	{
		buff += info->prec;
		if (*buff == '7' || *buff == '8'
		|| *(buff - 1) >= 'a' || *(buff - 1) >= 'A')
			*(buff - 1) += 1;
		else if (*buff == '9')
			*(buff - 1) = (info->cap) ? 'A' : 'a';
		i = 1;
		while (*(buff - i) > 'f')
		{
			*(buff - i) -= 1;
			i++;
			if (*(buff - i) == '.')
			{
				*(buff - i - 1) += 1;
				break ;
			}
			*(buff - i) += 1;
		}
	}
}

size_t					ldtoa_hex_form(t_ptf_info *info, uint64_t mantis,
						int64_t exp, char *buff)
{
	const char			*ptr = buff;
	size_t				ret;

	if (exp == -1022 || exp == -16381)
		*buff++ = '0';
	else
		*buff++ = '1';
	*buff++ = '.';
	ret = ft_ultoa_hex(mantis, buff, info->cap);
	ldtoa_hex_form_roundup(info, buff);
	buff += (info->prec < ret) ? info->prec : ret;
	if ((info->prec -= ret) > 0)
		while (info->prec--)
			*buff++ = '0';
	*buff++ = (info->cap) ? 'P' : 'p';
	if (exp < 0)
		*buff++ = '-';
	else
		*buff++ = '+';
	buff += ft_uitoa_dec((exp < 0) ? -exp : exp, buff);
	return (buff - ptr);
}
