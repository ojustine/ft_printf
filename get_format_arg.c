/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_format_arg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 13:56:38 by slynell           #+#    #+#             */
/*   Updated: 2020/02/19 13:56:53 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static inline void	get_width_n_precision(t_ptf_info *info)
{
	if (*info->fmt >= '0' && *info->fmt <= '9')
		while (*info->fmt >= '0' && *info->fmt <= '9')
			info->width = 10 * info->width + (*info->fmt++ - '0');
	else if (*info->fmt == '*' && info->fmt++)
	{
		info->width = va_arg(info->ap, int);
		if (info->width < 0 && (info->width = -info->width))
			info->flags |= FLAG_LEFT_ALIGN;
	}
	if (*info->fmt == '.' && info->fmt++)
	{
		info->prec = 0;
		info->flags |= FLAG_TRUNCATE;
		if (*info->fmt >= '0' && *info->fmt <= '9')
			while (*info->fmt >= '0' && *info->fmt <= '9')
				info->prec = 10 * info->prec + (*info->fmt++ - '0');
		else if (*info->fmt == '*')
		{
			info->prec = va_arg(info->ap, int);
			if (info->prec < 0)
				info->flags &= ~FLAG_TRUNCATE;
			info->prec = (info->prec < 0) ? 6 : info->prec;
			++info->fmt;
		}
	}
}

static inline void	get_size_modifier(t_ptf_info *info)
{
	while (*info->fmt)
	{
		if (*info->fmt == 'z')
			info->flags |= SIZE_SIZE_T;
		else if (*info->fmt == 'l')
			info->flags |= (*(info->fmt + 1) == 'l' && ++info->fmt)
			? SIZE_LLONG : SIZE_LONG;
		else if (*info->fmt == 'h')
			info->flags |= (*(info->fmt + 1) == 'h' && ++info->fmt)
			? SIZE_CHAR : SIZE_SHORT;
		else if (*info->fmt == 'j')
			info->flags |= SIZE_INTMAX;
		else if (*info->fmt == 'L')
			info->flags |= SIZE_LDBL;
		else if (*info->fmt == 't')
			info->flags |= SIZE_PTR;
		else
			break ;
		++info->fmt;
	}
}

static inline void	get_type(t_ptf_info *info)
{
	if (*info->fmt == 0)
		return ;
	info->cap = ft_strany("AXBFEGP", *info->fmt);
	if (*info->fmt == 'd' || *info->fmt == 'D' || *info->fmt == 'i')
		get_signed_arg(info, 10);
	else if (*info->fmt == 's')
		get_string_arg(info, info->flags & (SIZE_LONG | SIZE_LLONG));
	else if (*info->fmt == 'u' || *info->fmt == 'U')
		get_unsigned_arg(info, 10);
	else if (*info->fmt == 'o' || *info->fmt == 'O')
		get_unsigned_arg(info, 8);
	else if (*info->fmt == 'x' || *info->fmt == 'X')
		get_unsigned_arg(info, 16);
	else if (*info->fmt == 'b' || *info->fmt == 'B')
		get_unsigned_arg(info, 2);
	else if (ft_strany("afegAFEG", *info->fmt))
		get_floating_point_arg(info);
	else if (*info->fmt == 'c' || *info->fmt == 'C')
		get_char_arg(info, (*info->fmt == 'C'));
	else if (*info->fmt == 'S')
		get_string_arg(info, 1);
	else if (*info->fmt == 'p' && (info->flags |= FLAG_PTR))
		get_unsigned_arg(info, 16);
	else if (*info->fmt == 'n')
		*va_arg(info->ap, int*) = info->printed;
	else if (*info->fmt == '%')
	{
		if (info->flags & FLAG_TRUNCATE && info->prec == 0)
			info->prec = 1;
		do_print_string(info, "%", 1);
	}
	else if (*info->fmt == 'm')
		if (errno >= 0 && errno < 107)
			do_print_string(info, (char*)g_errors[errno],
			ft_strlen(g_errors[errno]));
		else
			do_print_string(info, (char*)g_errors[107],
			ft_strlen(g_errors[107]));
	else if (*info->fmt == '~' && ft_strnequ(++info->fmt, "{ANSI-", 6)
	&& ANSI_ECS_CODES && info->fd <= 2 && info->fd >= 0 && (info->fmt += 6))
		set_esc_code(info);
	else
		do_print(info, (char*)info->fmt, 1);
}

void				get_formatted_arg(t_ptf_info *info)
{
	info->flags = 0;
	info->width = 0;
	info->prec = 6;
	info->cap = 0;
	while (*info->fmt)
	{
		if (*info->fmt == '#')
			info->flags |= FLAG_ALT_FORM;
		else if (*info->fmt == '-')
			info->flags |= FLAG_LEFT_ALIGN;
		else if (*info->fmt == '+')
			info->flags |= FLAG_PLUS_SIGN;
		else if (*info->fmt == ' ')
			info->flags |= FLAG_BLANK_SIGN;
		else if (*info->fmt == '0')
			info->flags |= FLAG_ZERO_PAD;
		else
			break ;
		info->fmt++;
	}
	get_width_n_precision(info);
	get_size_modifier(info);
	get_type(info);
}
