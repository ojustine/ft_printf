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

void	print_fp_decimal_form(t_printf_info *info, t_fxd_dbl *fp)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	char					out[fp->int_len * R_LEN + info->prec + R_LEN + 1];
	register char			*ptr;

	ptr = &out[fp->int_len * R_LEN + info->prec + R_LEN];
	i = info->prec / R_LEN;
	while (--i > -fp->int_len)
	{
		j = R_LEN;
		if (i == -1)
			*ptr-- = '.';
		while (--j >= 0)
		{
			if (i - 1 == -fp->int_len && fp->val[D_F0 + i] == 0 && j < 8)
				break ;
			*ptr-- = (char)(fp->val[D_F0 + i] % 10 + '0');
			fp->val[D_F0 + i] /= 10;
		}
	}
	do_print(info, ptr + 1, (&out[fp->int_len * R_LEN + info->prec + R_LEN] - ptr));
}
