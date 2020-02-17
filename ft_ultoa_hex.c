#include "ft_printf.h"

size_t	ft_ultoa_hex(uint64_t value, char* buff, const int_fast16_t is_upper)
{
	static const char	digits[] = "0123456789abcdef0123456789ABCDEF";
	size_t				ret;

	ret = 1;
	buff[15] = digits[((value >> 60) & 0x0000000F) + (is_upper != 0) * 16];
	buff[14] = digits[((value >> 56) & 0x0000000F) + (is_upper != 0) * 16];
	buff[13] = digits[((value >> 52) & 0x0000000F) + (is_upper != 0) * 16];
	buff[12] = digits[((value >> 48) & 0x0000000F) + (is_upper != 0) * 16];
	buff[11] = digits[((value >> 44) & 0x0000000F) + (is_upper != 0) * 16];
	buff[10] = digits[((value >> 40) & 0x0000000F) + (is_upper != 0) * 16];
	buff[9] = digits[((value >> 36) & 0x0000000F) + (is_upper != 0) * 16];
	buff[8] = digits[((value >> 32) & 0x0000000F) + (is_upper != 0) * 16];
	buff[7] = digits[((value >> 28) & 0x0000000F) + (is_upper != 0) * 16];
	buff[6] = digits[((value >> 24) & 0x0000000F) + (is_upper != 0) * 16];
	buff[5] = digits[((value >> 20) & 0x0000000F) + (is_upper != 0) * 16];
	buff[4] = digits[((value >> 16) & 0x0000000F) + (is_upper != 0) * 16];
	buff[3] = digits[((value >> 12) & 0x0000000F) + (is_upper != 0) * 16];
	buff[2] = digits[((value >> 8) & 0x0000000F) + (is_upper != 0) * 16];
	buff[1] = digits[((value >> 4) & 0x0000000F) + (is_upper != 0) * 16];
	buff[0] = digits[((value) & 0x0000000F) + (is_upper != 0) * 16];
	while (buff[ret] != '0' && ret < 16)
		ret++;
	ft_strrev(buff, buff + ret - 1);
	buff[ret] = 0;
	return (ret);
}
