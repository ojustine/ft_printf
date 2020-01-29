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

void	print_fp_decimal_form(t_printf_info *info, uint32_t *fp,
		uint_fast16_t int_len)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	char					buff[int_len * R_LEN + info->prec + R_LEN + 1];
	register char			*ptr;
	size_t					to_print;

	ptr = &buff[int_len * R_LEN + info->prec + R_LEN];
	i = info->prec / R_LEN + 1;
	while (--i >= -int_len)
	{
		j = R_LEN;
		if (i == -1)
			*ptr-- = '.';
		while (--j >= 0)
		{
			*ptr-- = (char)(fp[D_F0 + i] % 10 + '0');
			fp[D_F0 + i] /= 10;
			if (i - 1 < -int_len && fp[D_F0 + i] == 0)
				break ;
		}
	}
	to_print = (&buff[int_len * R_LEN + info->prec + R_LEN] - ptr);
	to_print -= R_LEN - (info->prec % R_LEN);
	do_print(info, ptr + 1, to_print);
}
//void	pprint_fp_decimal_form(t_printf_info *info, uint32_t *fp,
//		uint_fast16_t int_len)
//{
//	register int_fast32_t	i;
//	register int_fast16_t	j;
//	char					buff[int_len * R_LEN + info->prec + R_LEN + 1];
//	register char			*ptr;
//
//	ptr = &buff[0];
//	i = ft_pow(10, ft_intlen(fp[D_I0 - int_len]));
//	j = 1;
//	while (i /= 10)
//	{
//		*ptr++ = (char)(fp[D_I0 - int_len] / R_LIMITER);
//	}
//	while (--i > -int_len)
//	{
//		j = R_LEN;
//		if (i == -1)
//			*ptr-- = '.';
//		while (--j >= 0)
//		{
//			if (i - 1 == -int_len && fp[D_F0 + i] == 0 && j < 8)
//				break ;
//			*ptr-- = (char)(fp[D_F0 + i] % 10 + '0');
//			fp[D_F0 + i] /= 10;
//		}
//	}
//	do_print(info, ptr + 1, &buff[int_len * R_LEN + info->prec + R_LEN] - ptr);
//}
