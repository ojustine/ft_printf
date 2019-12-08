#include "ft_printf.h"

static inline void	print_number(t_printf_info *info, uintmax_t num,
									const t_int16 base, char sign)
{
	const char			digits[] = "0123456789abcdef0123456789ABCDEF";
	register char		*ptr;
	char 				buff[BINARY_INT];
	size_t				i;

	if (info->flags & FLAG_ZERO_PAD && !(info->flags & FLAG_LEFT_ALIGN))
	{
		if (sign)
			put_in_buffer(info, &sign, 1);
		sign = 0;
	}
	i = 1;
	ptr = &buff[BINARY_INT - 1];
	while (i-- == 1 || num != 0)
	{
		*ptr-- = digits[(num % base) + info->capitals];
		num /= base;
	}

	i = (&buff[BINARY_INT - 1] - ptr);
	put_arg_in_buffer(info, ++ptr, i);
}

void				print_signed_number(t_printf_info *info, t_int16 base)
{
	char		sign;
	intmax_t	num;

	if (info->flags & SIZE_LONG || info->flags & SIZE_LLONG)
		num = (info->flags & SIZE_LLONG) ? va_arg(info->valist, long long)
										 : va_arg(info->valist, long);
	else if (info->flags & SIZE_SHORT || info->flags & SIZE_CHAR)
		num = (info->flags & SIZE_CHAR) ? (char)va_arg(info->valist, int)
										: (short)va_arg(info->valist, int);
	else if (info->flags & SIZE_INTMAX)
		num = (va_arg(info->valist, intmax_t));
	else if (info->flags & SIZE_SIZE_T)
		num = va_arg(info->valist, ssize_t);
	else
		num = va_arg(info->valist, int);
	if (num < 0)
	{
		sign = '-';
		num = -num;
	}
	else if (info->flags & FLAG_PLUS_SIGN || info->flags & FLAG_BLANK_SIGN)
			sign = (info->flags & FLAG_PLUS_SIGN) ? '+' : ' ';
	else
		sign = 0;
	print_number(info, num, base, sign);
}

void				print_unsigned_number(t_printf_info *info, t_int16 base)
{
	uintmax_t	num;
	char 		sign;

	if (info->flags & SIZE_LONG || info->flags & SIZE_LLONG)
		num = (info->flags & SIZE_LLONG)
		? va_arg(info->valist, unsigned long long)
		: va_arg(info->valist, unsigned long);
	else if (info->flags & SIZE_SHORT || info->flags & SIZE_CHAR)
		num = (info->flags & SIZE_CHAR)
		? (unsigned char)va_arg(info->valist, unsigned int)
		: (unsigned short)va_arg(info->valist, unsigned int);
	else if (info->flags & SIZE_INTMAX)
		num = (va_arg(info->valist, uintmax_t));
	else if (info->flags & SIZE_SIZE_T)
		num = va_arg(info->valist, size_t);
	else
		num = va_arg(info->valist, unsigned int);
	if ((info->flags & FLAG_PLUS_SIGN || info->flags & FLAG_BLANK_SIGN)
	&& base == 10)
		sign = (info->flags & FLAG_PLUS_SIGN) ? '+' : ' ';
	else
		sign = 0;
	print_number(info, num, base, sign);
}
