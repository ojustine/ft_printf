/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   number_processing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 11:23:05 by ojustine          #+#    #+#             */
/*   Updated: 2020/02/17 11:58:54 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static inline void		do_print_num(t_printf_info *info, uintmax_t num,
						const int_fast16_t base, char sign)
{
	char	buff[MAX_INT_BITS_NUM];
	size_t	to_print;

	if (info->flags & SIZE_LLONG || info->flags & SIZE_LONG
	|| info->flags & SIZE_INTMAX || info->flags & SIZE_SIZE_T)
	{
		if (base == 16)
			to_print = ft_ultoa_hex(num, buff, info->cap);
		else
			to_print = ft_ultoa_base(num, buff, base, info->cap);
	}
	else
	{
		if (base == 10)
			to_print = ft_uitoa_dec(num, buff);
		else if (base == 16)
			to_print = ft_ultoa_hex(num, buff, info->cap);
		else
			to_print = ft_ultoa_base(num, buff, base, info->cap);
	}
	if (info->flags & FLAG_TRUNCATE)
		info->flags &= ~FLAG_ZERO_PAD;
	info->width -= set_prefix_num(info, sign, base, to_print);
	do_print(info, buff, to_print);
	padding(info, info->width, ' ');
}

void					get_signed_arg(t_printf_info *info, int_fast16_t base)
{
	char				sign;
	intmax_t			num;
	uintmax_t			u_num;

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
	uintmax_t			num;

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
	do_print_num(info, num, base, 0);
}
