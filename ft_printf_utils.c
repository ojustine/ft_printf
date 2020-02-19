/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 11:52:46 by slynell           #+#    #+#             */
/*   Updated: 2020/02/19 14:03:37 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static inline int32_t	add_prefix(t_ptf_info *info, char *buff,
									const char sign, const int_fast16_t base)
{
	register int_fast16_t	ret;

	ret = 0;
	buff[0] = 0;
	if (sign)
		buff[ret++] = '-';
	else if (info->flags & (FLAG_PLUS_SIGN | FLAG_BLANK_SIGN))
		buff[ret++] = " +"[(info->flags & FLAG_PLUS_SIGN) != 0];
	if ((base != 10 && info->flags & FLAG_ALT_FORM)
	|| *info->fmt == 'a' || *info->fmt == 'A')
	{
		buff[ret++] = '0';
		if (base == 2)
			buff[ret++] = "bB"[info->cap];
		else if (base == 16)
			buff[ret++] = "xX"[info->cap];
	}
	return (ret);
}

void					padding(t_ptf_info *info, int_fast32_t pad_len,
								const char pad)
{
	static const char	zeroes_pad[] = "00000000";
	static const char	spaces_pad[] = "        ";
	char				*curr_pad;

	if (pad_len > 0)
	{
		curr_pad = (char*)((pad == '0') ? zeroes_pad : spaces_pad);
		while (pad_len >= 8)
		{
			do_print(info, curr_pad, 8);
			pad_len -= 8;
		}
		while (pad_len >= 4)
		{
			do_print(info, curr_pad, 4);
			pad_len -= 4;
		}
		while (pad_len--)
			do_print(info, curr_pad, 1);
	}
}

int32_t					set_prefix_num(t_ptf_info *info, const char sign,
						const int_fast16_t base, const int_fast32_t val_len)
{
	char				prefix[3];
	const int32_t		prefix_len = add_prefix(info, prefix, sign, base);
	register int32_t	zero_len;
	int32_t				padding_len;

	if (info->flags & FLAG_ZERO_PAD)
		zero_len = info->width - prefix_len - val_len;
	else
		zero_len = info->prec - val_len - (base == 8 && prefix[0] == '0');
	zero_len = (zero_len < 0) ? 0 : zero_len;
	padding_len = 0;
	if (!(info->flags & FLAG_LEFT_ALIGN))
	{
		padding_len = info->width - zero_len - val_len - prefix_len;
		padding(info, padding_len, ' ');
		do_print(info, prefix, prefix_len);
		padding(info, zero_len, '0');
	}
	else
	{
		do_print(info, prefix, prefix_len);
		padding(info, zero_len, '0');
	}
	return (val_len + prefix_len + zero_len + padding_len);
}

int32_t					set_prefix_fp(t_ptf_info *info, const char sign,
						const int_fast32_t val_len)
{
	register int32_t	len;
	char				prefix[3];
	char				pad;

	pad = (info->flags & FLAG_ZERO_PAD && !(info->flags & FLAG_LEFT_ALIGN))
			? '0' : ' ';
	len = add_prefix(info, prefix, sign,
	(*info->fmt == 'a' || *info->fmt == 'A') ? 16 : 10);
	if (info->width - (val_len + len) > 0 && !(info->flags & FLAG_LEFT_ALIGN))
		if (info->flags & FLAG_ZERO_PAD)
		{
			do_print(info, prefix, len);
			padding(info, info->width - (val_len + len), pad);
			len += info->width - (val_len + len);
		}
		else
		{
			padding(info, info->width - (val_len + len), pad);
			do_print(info, prefix, len);
			len += info->width - (val_len + len);
		}
	else
		do_print(info, prefix, len);
	return (val_len + len);
}

void					set_esc_code(t_ptf_info *info)
{
	info->flush(info);
	if (ft_strnequ(info->fmt, "BLD}", 4))
		write(info->fd, ANSI_BOLD, 7);
	else if (ft_strnequ(info->fmt, "UND}", 4))
		write(info->fd, ANSI_UNDERLINE, 7);
	else if (ft_strnequ(info->fmt, "REV}", 4))
		write(info->fd, ANSI_REVERSED, 7);
	else if (ft_strnequ(info->fmt, "B-R}", 4))
		write(info->fd, ANSI_BACK_RED, 7);
	else if (ft_strnequ(info->fmt, "B-G}", 4))
		write(info->fd, ANSI_BACK_GRN, 7);
	else if (ft_strnequ(info->fmt, "B-Y}", 4))
		write(info->fd, ANSI_BACK_YEL, 7);
	else if (ft_strnequ(info->fmt, "B-B}", 4))
		write(info->fd, ANSI_BACK_BLU, 7);
	else if (ft_strnequ(info->fmt, "RED}", 4))
		write(info->fd, ANSI_FONT_RED, 7);
	else if (ft_strnequ(info->fmt, "GRN}", 4))
		write(info->fd, ANSI_FONT_GRN, 7);
	else if (ft_strnequ(info->fmt, "YEL}", 4))
		write(info->fd, ANSI_FONT_YEL, 7);
	else if (ft_strnequ(info->fmt, "BLU}", 4))
		write(info->fd, ANSI_FONT_BLU, 7);
	else if (ft_strnequ(info->fmt, "RES}", 4))
		write(info->fd, ANSI_RESET, 7);
}
