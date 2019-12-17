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

static inline t_int16	add_prefix(t_printf_info *info, const t_int16 base,
									char sign)
{
	t_int16	ret;

	ret = 0;
	if (sign)
	{
		do_print(info, &sign, 1);
		ret++;
	}
	if (base != 10 && info->flags & FLAG_ALT_FORM)
	{
		if (base == 8)
		{
			do_print(info, "0", 1);
			return (ret + 1);
		}
		else if (base == 2)
			do_print(info, info->capitals ? "0B" : "0b", 2);
		else if (base == 16)
			do_print(info, info->capitals ? "0X" : "0x", 2);
		return (ret + 2);
	}
	return (ret);
}

static inline void		do_print_num(t_printf_info *info, uintmax_t num,
									   const t_int16 base, char sign)
{
	const char			digits[] = "0123456789abcdef0123456789ABCDEF";
	register char		*ptr;
	char 				buff[BIN_MAX_INT];
	t_int16				num_len;

	num_len = 1;
	ptr = &buff[BIN_MAX_INT - 1];
	while (num_len-- == 1 || num != 0)
	{
		*ptr-- = digits[(num % base) + info->capitals];
		num /= base;
	}
	num_len = (&buff[BIN_MAX_INT - 1] - ptr);
	info->width -= (sign) ? 1 : 0;
	if (base != 10 && (info->flags & FLAG_ALT_FORM))
		info->width -= (base == 8) ? 1 : 2;
	if (!(info->flags & FLAG_LEFT_ALIGN) && ((info->width -= MAX(info->prec,
		num_len)) > 0))
		padding(info);
	info->width -= add_prefix(info, base, sign) + info->prec;
	if (info->flags & FLAG_TRUNCATE && (info->prec -= num_len) > 0)
		while (info->prec--)
			do_print(info, "0", 1);
	do_print(info, ++ptr, num_len);
	//if (info->width > 0)
		padding(info);
}

void					print_signed_number(t_printf_info *info, t_int16 base)
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
	do_print_num(info, num, base, sign);
}

void					print_unsigned_number(t_printf_info *info, t_int16 base)
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
	do_print_num(info, num, base, sign);
}