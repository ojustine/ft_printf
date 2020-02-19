/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_processing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 12:01:33 by slynell           #+#    #+#             */
/*   Updated: 2020/02/17 12:11:55 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void					do_print_string(t_ptf_info *info, char *str,
						size_t size)
{
	size_t				to_print;
	char				pad;

	pad  = (info->flags & FLAG_ZERO_PAD	&& !(info->flags & FLAG_LEFT_ALIGN))
	? '0' : ' ';
	if (info->flags & FLAG_TRUNCATE)
		to_print = (info->prec < size) ? info->prec : size;
	else
		to_print = size;
	if (info->width > 0 && (info->width -= to_print) > 0)
	{
		info->flags &= ~FLAG_ZERO_PAD;
		if (info->flags & FLAG_LEFT_ALIGN)
		{
			do_print(info, str, to_print);
			padding(info, info->width, pad);
		}
		else
		{
			padding(info, info->width, pad);
			do_print(info, str, to_print);
		}
	}
	else
		do_print(info, str, to_print);
}

static inline int32_t	print_wchar(t_ptf_info *info, uint32_t wc,
									 int16_t is_single)
{
	char				utf_8[4];
	size_t				bytes;

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

void					get_char_arg(t_ptf_info *info, int16_t is_wide_char)
{
	char				c;
	wint_t				wc;

	if (info->prec == 0)
		info->prec = 1;
	if (is_wide_char)
	{
		wc = va_arg(info->ap, wint_t);
		print_wchar(info, wc, 1);
	}
	else
	{
		c = (char)va_arg(info->ap, int);
		do_print_string(info, &c, 1);
	}
}

void					get_string_arg(t_ptf_info *info,
									   int16_t is_wide_string)
{
	char				*str;
	wint_t				*wstr;
	int					wstr_len;

	if (is_wide_string)
		if (!(wstr = va_arg(info->ap, wint_t*)))
			do_print_string(info, "(null)", 6);
		else
		{
			wstr_len = (int)(ft_wstrlen(wstr));
			while (*wstr && (wstr_len -= print_wchar(info, *wstr++, 0)) > 0)
				if (wstr_len <= 4 && (info->flags & FLAG_LEFT_ALIGN))
					wstr_len -= print_wchar(info, *wstr++, 0);
		}
	else
	{
		if (!(str = va_arg(info->ap, char*)))
			do_print_string(info, "(null)", 6);
		else
			do_print_string(info, str, ft_strlen(str));
	}
}
