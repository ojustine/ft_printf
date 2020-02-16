/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   number_processing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojustine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 11:23:05 by ojustine          #+#    #+#             */
/*   Updated: 2019/12/10 11:23:09 by ojustine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static inline void		do_print_num(t_printf_info *info, uintmax_t num,
						const int_fast16_t base, char sign)
{
	const char			digits[] = "0123456789abcdef0123456789ABCDEF";
	register char		*ptr;
	char				buff[MAX_INT_BITS_NUM];
	int_fast16_t		num_len;

	ptr = &buff[MAX_INT_BITS_NUM - 1];
	*ptr-- = digits[(num % base) + info->cap * 16];
	num /= base;
	while (num != 0)
	{
		*ptr-- = digits[(num % base) + info->cap * 16];
		num /= base;
	}
	num_len = (&buff[MAX_INT_BITS_NUM - 1] - ptr);
	if (info->flags & FLAG_TRUNCATE)
		info->flags &= ~FLAG_ZERO_PAD;
	info->width -= set_prefix_num(info, sign, base, num_len);
	do_print(info, ++ptr, num_len);
	padding(info, info->width, ' ');
}

void					get_signed_arg(t_printf_info *info, int_fast16_t base)
{
	char		sign;
	intmax_t	num;
	uintmax_t	u_num;

	if (info->flags & SIZE_LLONG)
		num = va_arg(info->ap, long long);
	else if (info->flags & SIZE_LONG)
		num = va_arg(info->ap, long);
	else if (info->flags & SIZE_SHORT)
		num = (short)va_arg(info->ap, int);
	else if (info->flags & SIZE_CHAR)
		num = (char)va_arg(info->ap, int);
	else if (info->flags & SIZE_INTMAX)
		num = (va_arg(info->ap, intmax_t));
	else if (info->flags & SIZE_SIZE_T)
		num = va_arg(info->ap, ssize_t);
	else
		num = va_arg(info->ap, int);
	sign = (char)(num < 0);
	u_num = (num < 0) ? -num : num;
	do_print_num(info, u_num, base, sign);
}

void					get_unsigned_arg(t_printf_info *info, int_fast16_t base)
{
	uintmax_t	num;
	char		sign;

	if (info->flags & SIZE_LLONG)
		num = va_arg(info->ap, unsigned long long);
	else if (info->flags & SIZE_LONG)
		num = va_arg(info->ap, unsigned long);
	else if (info->flags & SIZE_SHORT)
		num = (unsigned short)va_arg(info->ap, unsigned int);
	else if (info->flags & SIZE_CHAR)
		num = (unsigned char)va_arg(info->ap, unsigned int);
	else if (info->flags & SIZE_INTMAX)
		num = (va_arg(info->ap, uintmax_t));
	else if (info->flags & SIZE_SIZE_T)
		num = va_arg(info->ap, size_t);
	else
		num = va_arg(info->ap, unsigned int);
	sign = 0;
	do_print_num(info, num, base, sign);
}
