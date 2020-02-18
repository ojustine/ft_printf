#include "libptf.h"

size_t	ft_uitoa_hex(const uint32_t value, char* buff,
		const int_fast16_t is_upper)
{
	static const char	digits[] = "0123456789abcdef0123456789ABCDEF";
	register size_t		ret;

	ret = 7;
	buff[7] = digits[((value >> 28) & 0x0000000F) + (is_upper != 0) * 16];
	buff[6] = digits[((value >> 24) & 0x0000000F) + (is_upper != 0) * 16];
	buff[5] = digits[((value >> 20) & 0x0000000F) + (is_upper != 0) * 16];
	buff[4] = digits[((value >> 16) & 0x0000000F) + (is_upper != 0) * 16];
	buff[3] = digits[((value >> 12) & 0x0000000F) + (is_upper != 0) * 16];
	buff[2] = digits[((value >> 8) & 0x0000000F) + (is_upper != 0) * 16];
	buff[1] = digits[((value >> 4) & 0x0000000F) + (is_upper != 0) * 16];
	buff[0] = digits[((value) & 0x0000000F) + (is_upper != 0) * 16];
	while (buff[ret] == '0' && ret > 0)
		ret--;
	ft_strrev(buff, buff + ret);
	buff[ret + 1] = 0;
	return (ret + 1);
}
