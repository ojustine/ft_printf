#include "ft_printf.h"

int32_t	add_prefix(t_printf_info *info, const int_fast16_t base,
					char* buff)
{
	int_fast16_t	ret;

	ret = 0;
//	if (sign)
//	{
//		do_print(info, &sign, 1);
//		ret++;
//	}

	if (base != 10 && info->flags & FLAG_ALT_FORM)
	{
		if (base == 8)
		{
			do_print(info, "0", 1);
			return (ret + 1);
		}
		else if (base == 2)
			do_print(info, info->cap ? "0B" : "0b", 2);
		else if (base == 16)
			do_print(info, info->cap ? "0X" : "0x", 2);
		return (ret + 2);
	}
	return (ret);
}

static inline void	padding(t_printf_info *info, int_fast32_t pad_len)
{
	const char	zero_pad[] = "0000";
	const char	blank_pad[] = "    ";
	char		*curr_pad;

	if (pad_len > 0)
	{
		curr_pad = (char*)(info->flags & FLAG_ZERO_PAD ? zero_pad : blank_pad);
		while (pad_len >= 4)
		{
			do_print(info, curr_pad, 4);
			pad_len -= 4;
		}
		while (pad_len--)
			do_print(info, curr_pad, 1);
	}
}

void				add_prefix_fp(t_printf_info *info, char sign,
					int_fast32_t val_len)
{
	int32_t	ret;

	ret = 0;
	if (sign)
		do_print(info, "-", ++ret);
	else if (info->flags & FLAG_PLUS_SIGN || info->flags & FLAG_BLANK_SIGN)
		do_print(info, info->flags & FLAG_PLUS_SIGN ? "+" : " ", ++ret);
	if (*info->fmt == 'a' || *info->fmt == 'A')
		ret += 2;
	if (info->width - (val_len + ret) > 0 && !(info->flags & FLAG_LEFT_ALIGN))
		if (info->flags & FLAG_ZERO_PAD)
		{
			if (*info->fmt == 'a' || *info->fmt == 'A')
				do_print(info, info->cap ? "0X" : "0x", 2);
			padding(info, info->width -= val_len + ret);
		}
		else
		{
			padding(info, info->width -= val_len + ret);
			if (*info->fmt == 'a' || *info->fmt == 'A')
				do_print(info, info->cap ? "0X" : "0x", 2);
		}
	else if (*info->fmt == 'a' || *info->fmt == 'A')
		do_print(info, info->cap ? "0X" : "0x", 2);
}


