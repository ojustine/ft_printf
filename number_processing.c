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

static inline void		do_print_num(t_ptf_info *info, uintmax_t value,
									   const int_fast16_t base, char sign)
{
	char	buff[MAX_INT_BITS_NUM];
	size_t	to_print;

	if (info->flags & (SIZE_LLONG | SIZE_LONG | SIZE_INTMAX | SIZE_SIZE_T))
		if (base == 16)
			to_print = ft_ultoa_hex(value, buff, info->cap);
		else
			to_print = ft_ultoa_base(value, buff, base, info->cap);
	else
		if (base == 10)
			to_print = ft_uitoa_dec(value, buff);
		else if (base == 16)
			to_print = ft_uitoa_hex(value, buff, info->cap);
		else
			to_print = ft_uitoa_base(value, buff, base, info->cap);
	if (info->flags & (FLAG_TRUNCATE | FLAG_LEFT_ALIGN))
		info->flags &= ~FLAG_ZERO_PAD;
	if ((value == 0 && base != 8 && !(info->flags & SIZE_PTR))
	|| (value == 0 && base == 8 && !(info->flags & FLAG_TRUNCATE)))
		info->flags &= ~FLAG_ALT_FORM;
	if (value == 0 && info->flags & FLAG_TRUNCATE && info->prec == 0)
		to_print = 0;
	info->width -= set_prefix_num(info, sign, base, to_print);
	do_print(info, buff, to_print);
	padding(info, info->width, ' ');
}

void					get_signed_arg(t_ptf_info *info, int_fast16_t base)
{
	char				sign;
	intmax_t			value;
	uintmax_t			u_value;

	if (info->flags & SIZE_LLONG)
		value = va_arg(info->ap, long long);
	else if (info->flags & SIZE_LONG)
		value = va_arg(info->ap, long);
	else if (info->flags & SIZE_SIZE_T)
		value = va_arg(info->ap, ssize_t);
	else if (info->flags & SIZE_INTMAX)
		value = (va_arg(info->ap, intmax_t));
	else if (info->flags & SIZE_SHORT)
		value = (short)va_arg(info->ap, int);
	else if (info->flags & SIZE_CHAR)
		value = (char)va_arg(info->ap, int);
	else
		value = va_arg(info->ap, int);
	sign = (char)(value < 0);
	u_value = (value < 0) ? -value : value;
	if (!(info->flags & FLAG_TRUNCATE))
		info->prec = 0;
	do_print_num(info, u_value, base, sign);
}

void					get_unsigned_arg(t_ptf_info *info, int_fast16_t base)
{
	uintmax_t			value;

	if (info->flags & SIZE_LLONG)
		value = va_arg(info->ap, unsigned long long);
	else if (info->flags & SIZE_LONG)
		value = va_arg(info->ap, unsigned long);
	else if (info->flags & SIZE_SIZE_T)
		value = va_arg(info->ap, size_t);
	else if (info->flags & SIZE_INTMAX)
		value = (va_arg(info->ap, uintmax_t));
	else if (info->flags & SIZE_SHORT)
		value = (unsigned short)va_arg(info->ap, unsigned int);
	else if (info->flags & SIZE_CHAR)
		value = (unsigned char)va_arg(info->ap, unsigned int);
	else
		value = va_arg(info->ap, unsigned int);
	info->flags &= (~FLAG_PLUS_SIGN & ~FLAG_BLANK_SIGN);
	if (!(info->flags & FLAG_TRUNCATE))
		info->prec = 0;
	do_print_num(info, value, base, 0);
}
