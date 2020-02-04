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

	i = (info->prec <= R_LEN * 115) ? info->prec / R_LEN + 1 : 116;
	it = (int)(fp[D_F0 + i] / ft_pow(10, (R_LEN - (info->prec % R_LEN)))) % 10;
}

size_t					print_fp_dec_form(t_printf_info *info, t_fxd *fp, char *buff)
{
	register int_fast16_t	i;
	register int_fast16_t	j;

	i = -fp->int_len;
	fp->val[D_F0 + i] *= ft_pow(10, ft_intlen(R_LEN - fp->val[D_F0 + i]));
	while ((fp->val[D_F0 + i] *= 10 > 0) && i < 0)
	{
		*buff++ = (char)(fp->val[D_F0 + i] / R_LIMITER + '0');
		fp->val[D_F0 + i] %= R_LIMITER;
	}
	while (++i < 0)
	{
		*buff++ = fp->val[D_F0 - fp->int_len];
	}
//	roundup(info, fp);
	while (--i >= -fp->int_len)
	{
		j = R_LEN;
		if (i == -1 && info->prec)
			*ptr-- = '.';
		while (--j >= 0)
		{
			*ptr-- = (char)(fp->val[D_F0 + i] % 10 + '0');
			fp->val[D_F0 + i] /= 10;
			if (i - 1 < -fp->int_len && fp->val[D_F0 + i] == 0)
				break ;
		}
	}
	do_print(info, ptr + 1, (&buff[D_LEN] - ptr) - (R_LEN - (info->prec % R_LEN)));
}

void					print_fp_exp_form(t_printf_info *info, t_fxd *fp)
{
	t_fxd			*mul;
	int_fast16_t	offset;
	int_fast16_t	index;

	index = 0;
	if (fp->int_len == 0 && fp->frc_len > 0)
		while (fp->val[D_F0 + index] == 0 && index < fp->frc_len)
			index++;
	else if (fp->int_len > 0)
		index = -fp->int_len;
	offset = ft_intlen(fp->val[D_F0 + index]) - 1;
	mul = fxd_new((index > 0) ? index : 0, 0);
	mul->frc_len = (index > 0) ? index + 1 : 0;
	mul->int_len = (index < 0) ? -index : 0;
	mul->val[D_F0 + index] = ft_pow(10, offset);
	fxd_dbl_mul(fp, fp, mul);
}
