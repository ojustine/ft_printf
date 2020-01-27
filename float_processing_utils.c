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

void	print_fp_decimal_form(t_printf_info *info, t_fxd_pattern *fp)
{
	register int16_t	i;
	register int32_t	rank;
	const int16_t		len = ft_intlen(fp->val[D_I0 - fp->int_len - 1]);
	char				output[fp->int_len * R_LEN + info->prec + R_LEN + 1];
	register char		*ptr;

	i = 0;
	ptr = &output[fp->int_len * R_LEN + info->prec + R_LEN];
	while (i++ < fp->int_len + info->prec)
	{
		output[i] = fp->val[--fp->int_len];
	}
}
