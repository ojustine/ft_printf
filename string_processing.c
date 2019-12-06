#include "ft_printf.h"

inline int		print_wchar(t_printf_info *info, uint32_t wc)
{
	char	uni_char[4];
	size_t	bytes;

	bytes = 0;
	if (wc < 0x80)
		uni_char[bytes++] = wc;
	else if (wc < 0x800)
		uni_char[bytes++] = (char)((wc >> 0x6U) | 0xc0U);
	else if (wc < 0x10000)
	{
		uni_char[bytes++] = (char)((wc >> 0xcU) | 0xe0U);
		uni_char[bytes++] = (char)((wc >> 0x6U) | 0x80U);
	}
	else
	{
		uni_char[bytes++] = (char)((wc >> 0x12U) | 0xf0U);
		uni_char[bytes++] = (char)((wc >> 0xcU) | 0x80U);
		uni_char[bytes++] = (char)((wc >> 0x6U) | 0x80U);
	}
	if (wc >= 0x80)
		uni_char[bytes++] = (char)((wc & 0x3fU) | 0x80U);
	if (MB_CUR_MAX >= bytes)
		put_in_buffer(info, uni_char, bytes);
	return (bytes);
}
