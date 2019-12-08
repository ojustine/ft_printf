#include "ft_printf.h"

t_int32		print_wchar(t_printf_info *info, uint32_t wc, t_int16 is_single)
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
	if (wc >= 0x80)
		utf_8[bytes++] = (char)((wc & 0x3fU) | 0x80U);
	if (MB_LEN_MAX >= bytes)
		is_single ? put_arg_in_buffer(info, utf_8, bytes)
					: put_in_buffer(info, utf_8, bytes);
	return (bytes);
}

void		print_wstring(t_printf_info *info)
{
	wchar_t		*wstr;
	int			wstr_len;
	int			wchar_len;

	if (!(wstr = va_arg(info->valist, wchar_t*)))
		put_arg_in_buffer(info, "(null)", 6);
	else
	{
		wstr_len = (int)(ft_wstrlen(wstr));
		wchar_len = 0;
		while (*wstr && (wstr_len -= wchar_len) > 0)
			wchar_len = print_wchar(info, *wstr++, 0);
	}
}

void		print_string(t_printf_info *info)
{
	char 	*str;

	if (!(str = va_arg(info->valist, char*)))
		put_arg_in_buffer(info, "(null)", 6);
	else
		put_arg_in_buffer(info, str, ft_strlen(str));
}
