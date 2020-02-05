#include "ft_printf.h"

static inline void		padding(t_printf_info *info)
{
	const char	zero_pad[] = "0000";
	const char	blank_pad[] = "    ";
	char		*curr_pad;

	if (info->width > 0)
	{
		curr_pad = (char *)((info->flags & FLAG_ZERO_PAD
							&& !(info->flags & FLAG_TRUNCATE)
							&& !(info->flags & FLAG_LEFT_ALIGN))
							? zero_pad : blank_pad);
		while (info->width >= 4)
		{
			do_print(info, curr_pad, 4);
			info->width -= 4;
		}
		while (info->width--)
			do_print(info, curr_pad, 1);
	}
}

static inline void		roundup(t_printf_info *info, char *fp)
{
	uint32_t 				it;
	register int_fast16_t	i;

	i = (info->prec <= RANK_LEN * 115) ? info->prec / RANK_LEN + 1 : 116;
	it = (int)(fp[D_F0 + i] / ft_pow(10, (RANK_LEN - (info->prec % RANK_LEN)))) % 10;
}

static inline size_t	fxd_ftoa_dec_form_frac_part(t_printf_info *info, t_fxd *fp, char *buff)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	register int_fast32_t	prec;
	const char *ptr = buff;

	if ((*info->fmt == 'g' || *info->fmt == 'G')
	&& (info->prec == 0 || fp->frc_len == 0))
		return (0);
	*buff++ = '.';
	i = -1;
	prec = 0;
	while (++i < fp->frc_len)
	{
		j = RANK_LEN;
		while ((prec < info->prec) && --j >= 0)
		{
			prec = i * RANK_LEN + RANK_LEN - j;
			*buff++ = (char)(fp->val[D_F0 + i] / RANK_TOP + '0');
			fp->val[D_F0 + i] %= RANK_TOP;
			fp->val[D_F0 + i] *= 10;
		}
	}
	while (prec++ < info->prec)
		*buff++ = '0';
	return (buff - ptr);
}

size_t					fxd_ftoa_dec_form(t_printf_info *info, t_fxd *fp, char *buff)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	const char *ptr = buff;

	i = -fp->int_len;
	if (i == 0)
		*buff++ = '0';
	else
	{
		j = ft_intlen(fp->val[D_F0 + i]);
		fp->val[D_F0 + i] *= ft_pow(10, RANK_LEN - j);
		while (i < 0)
		{
			while (--j >= 0)
			{
				*buff++ = (char)(fp->val[D_F0 + i] / RANK_TOP + '0');
				fp->val[D_F0 + i] %= RANK_TOP;
				fp->val[D_F0 + i] *= 10;
			}
			j = RANK_LEN;
			i++;
		}
	}
	return ((buff - ptr) + fxd_ftoa_dec_form_frac_part(info, fp, buff));
}

size_t					fxd_ftoa_exp_form(t_printf_info *info, t_fxd *fp, char *buff)
{
	t_fxd			*mul;
	int_fast16_t	offset;
	int_fast16_t	index;
	const char		*ptr = buff;

	index = 0;
	if (fp->int_len == 0 && fp->frc_len > 0)
		while (fp->val[D_F0 + index] == 0 && index < fp->frc_len)
			index++;
	else if (fp->int_len > 0)
		index = -fp->int_len;
	else
		return (fxd_ftoa_dec_form(info, fp, buff));
	offset = ft_intlen(fp->val[D_F0 + index]) - 1;
	mul = fxd_new((index < 0) ? index : 0, 0);
	mul->frc_len = (index < 0) ? -index : 0;
	mul->int_len = (index >= 0) ? index + 1 : 0;
	mul->val[D_F0 - 1 - index] = ft_pow(10, RANK_LEN - offset);
	fxd_dbl_mul(fp, fp, mul);
	return (fxd_ftoa_dec_form(info, fp, buff));
}
