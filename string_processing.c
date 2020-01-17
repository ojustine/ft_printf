#include "ft_printf.h"

static inline void			padding(t_printf_info *info)
{
	const char	zero_pad[] = "0000";
	const char	blank_pad[] = "    ";
	char		*curr_pad;

	curr_pad = (char*)((info->flags & FLAG_ZERO_PAD) ? zero_pad : blank_pad);
	while (info->width >= 4)
	{
		do_print(info, curr_pad, 4);
		info->width -= 4;
	}
	while (info->width--)
		do_print(info, curr_pad, 1);
}

static inline void			do_print_string(t_printf_info *info, char *data,
											  size_t size)
{
	t_u32	to_print;

	if (info->flags & FLAG_TRUNCATE)
		to_print = (info->prec < size) ? info->prec : size;
	else
		to_print = size;
	if (info->width > 0 && (info->width -= to_print) > 0)
	{
		if (info->flags & FLAG_LEFT_ALIGN)
		{
			info->flags &= ~FLAG_ZERO_PAD;
			do_print(info, data, to_print);
			padding(info);
		}
		else
		{
			padding(info);
			do_print(info, data, to_print);
		}
	}
	else
		do_print(info, data, to_print);
}


static inline t_s32		print_wchar(t_printf_info *info, uint32_t wc,
									   t_s16 is_single)
{
	char	utf_8[4];
	size_t	bytes;

	bytes = 0;
	if (wc <= 0x7F)
		utf_8[bytes++] = wc;
	else if (wc <= 0x7FF)
		utf_8[bytes++] = (char)((wc >> 0x6U) | 0xc0U);
	else if (wc <= 0xFFFF)
	{
		utf_8[bytes++] = (char)((wc >> 0xcU) | 0xe0U);
		utf_8[bytes++] = (char)((wc >> 0x6U) | 0x80U);
	}
	else if (wc <= 0x10FFFF)
	{
		utf_8[bytes++] = (char)((wc >> 0x12U) | 0xf0U);
		utf_8[bytes++] = (char)((wc >> 0xcU) | 0x80U);
		utf_8[bytes++] = (char)((wc >> 0x6U) | 0x80U);
	}
	if (wc > 0x7F)
		utf_8[bytes++] = (char)((wc & 0x3fU) | 0x80U);
	is_single ? do_print_string(info, utf_8, bytes)
				: do_print(info, utf_8, bytes);
	return (bytes);
}

void						print_char(t_printf_info *info,
									   t_s16 is_wide_char)
{
	char	c;
	wchar_t wc;

	if (is_wide_char)
	{
		wc = va_arg(info->ap, wchar_t);
		print_wchar(info, wc, 1);
	}
	else
	{
		c = (char)va_arg(info->ap, int);
		do_print_string(info, &c, 1);
	}
}

void						print_string(t_printf_info *info,
										 t_s16 is_wide_string)
{
	char		*str;
	wchar_t		*wstr;
	int			wstr_len;

	if (is_wide_string)
		if (!(wstr = va_arg(info->ap, wchar_t*)))
			do_print_string(info, "(null)", 6);
		else
		{
			wstr_len = (int)(ft_wstrlen(wstr));
			while (*wstr && (wstr_len -= print_wchar(info, *wstr++, 0)) > 0)
				if (wstr_len <= 4 && (info->flags & FLAG_LEFT_ALIGN))
					wstr_len -= print_wchar(info, *wstr++, 0);
		}
	else
		if (!(str = va_arg(info->ap, char*)))
			do_print_string(info, "(null)", 6);
		else
			do_print_string(info, str, ft_strlen(str));
}
