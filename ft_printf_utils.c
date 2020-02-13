#include "ft_printf.h"

int32_t	add_prefix(t_printf_info *info, char *buf, char sign, const int_fast16_t base)
{
	int_fast16_t	ret;

	ret = 0;
	if (sign)
		buf[ret++] = '-';
	else if (info->flags & FLAG_PLUS_SIGN || info->flags & FLAG_BLANK_SIGN)
		buf[++ret] = " +"[info->flags & FLAG_PLUS_SIGN];
	if ((base != 10 && info->flags & FLAG_ALT_FORM) || *info->fmt == 'a'
													|| *info->fmt == 'A')
	{
		buf[ret++] = '0';
		if (base == 2)
			buf[ret++] = "bB"[info->cap];
		else if (base == 16)
			buf[ret++] = "xX"[info->cap];
	}
	return (ret);
}

void				padding(t_printf_info *info, int_fast32_t pad_len)
{
	const char	zero_pad[] = "0000";
	const char	blank_pad[] = "    ";
	char		*curr_pad;

	if (pad_len > 0)
	{
		curr_pad = (char*)(info->flags & FLAG_ZERO_PAD
							&& !(info->flags & FLAG_LEFT_ALIGN)
							? zero_pad : blank_pad);
		while (pad_len >= 4)
		{
			do_print(info, curr_pad, 4);
			pad_len -= 4;
		}
		while (pad_len--)
			do_print(info, curr_pad, 1);
	}
}

int32_t				set_prefix_fp(t_printf_info *info, char sign,
					int_fast32_t val_len)
{
	int32_t	len;
	char 	prefix[3];

	len = add_prefix(info, prefix, sign, 16);
	if (info->width - (val_len + len) > 0 && !(info->flags & FLAG_LEFT_ALIGN))
	{
		if (info->flags & FLAG_ZERO_PAD)
		{
			do_print(info, prefix, len);
			padding(info, info->width - (val_len + len));
			len += info->width - (val_len + len);
		}
		else
		{
			padding(info, info->width - (val_len + len));
			do_print(info, prefix, len);
			len += info->width - (val_len + len);
		}
	}
	else
		do_print(info, prefix, len);
	return (val_len + len);
}
