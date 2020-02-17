#include "ft_printf.h"

static inline int32_t	add_prefix(t_printf_info *info, char *buf,
						const char sign, const int_fast16_t base)
{
	int_fast16_t	ret;

	ret = 0;
	if (sign)
		buf[ret++] = '-';
	else if (info->flags & FLAG_PLUS_SIGN || info->flags & FLAG_BLANK_SIGN)
		buf[++ret] = " +"[info->flags & FLAG_PLUS_SIGN];
	if (base != 10 && info->flags & FLAG_ALT_FORM)
	{
		buf[ret++] = '0';
		if (base == 2)
			buf[ret++] = "bB"[info->cap];
		else if (base == 16)
			buf[ret++] = "xX"[info->cap];
	}
	return (ret);
}

void					padding(t_printf_info *info, int_fast32_t pad_len,
						const char pad)
{
	static const char	zeroes_pad[] = "00000000";
	static const char	spaces_pad[] = "        ";
	char				*curr_pad;

	if (pad_len > 0)
	{
		curr_pad = (char*)((pad == '0') ? zeroes_pad : spaces_pad);
		while (pad_len >= 8)
		{
			do_print(info, curr_pad, 8);
			pad_len -= 8;
		}
		while (pad_len >= 4)
		{
			do_print(info, curr_pad, 4);
			pad_len -= 4;
		}
		while (pad_len--)
			do_print(info, curr_pad, 1);
	}
}

int32_t					set_prefix_num(t_printf_info *info, const char sign,
						const int_fast16_t base, const int_fast32_t val_len)
{
	int32_t		prefix_len;
	int32_t		zeroes_len;
	int32_t		padding_len;
	char 		prefix[3];

	prefix_len = add_prefix(info, prefix, sign, base);
	zeroes_len = (info->flags & FLAG_TRUNCATE && info->prec > val_len)
											? info->prec - val_len : 0;
	padding_len = 0;
	if (!(info->flags & FLAG_LEFT_ALIGN))
	{
		padding_len = info->width - zeroes_len - val_len - prefix_len;
		padding(info, padding_len, ' ');
		do_print(info, prefix, prefix_len);
		padding(info, zeroes_len, '0');
	}
	else
	{
		do_print(info, prefix, prefix_len);
		padding(info, zeroes_len, '0');
	}
	return (val_len + prefix_len + zeroes_len + padding_len);
}

int32_t					set_prefix_fp(t_printf_info *info, const char sign,
						const int_fast32_t val_len)
{
	int32_t		len;
	char 		prefix[1];
	const char	pad = (info->flags & FLAG_ZERO_PAD
				&& !(info->flags & FLAG_LEFT_ALIGN)) ? '0' : ' ';

	len = add_prefix(info, prefix, sign, 10);
	if (info->width - (val_len + len) > 0 && !(info->flags & FLAG_LEFT_ALIGN))
	{
		if (info->flags & FLAG_ZERO_PAD)
		{
			do_print(info, prefix, len);
			padding(info, info->width - (val_len + len), pad);
			len += info->width - (val_len + len);
		}
		else
		{
			padding(info, info->width - (val_len + len), pad);
			do_print(info, prefix, len);
			len += info->width - (val_len + len);
		}
	}
	else
		do_print(info, prefix, len);
	return (val_len + len);
}
