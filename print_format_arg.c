#include "ft_printf.h"

static inline void	get_width_n_precision(t_printf_info *info)
{
	if (*info->fmt >= '0' && *info->fmt <= '9')
		while (*info->fmt >= '0' && *info->fmt <= '9')
			info->width = 10 * info->width + (*info->fmt++ - '0');
	else if (*info->fmt == '*')
	{
		info->width = va_arg(info->valist, t_int32);
		++info->fmt;
		if (info->width < 0 && (info->width = -info->width))
			info->flags |= FLAG_LEFT_ALIGN;
	}
	if (*info->fmt == '.')
	{
		++info->fmt;
		info->prec = 0;
		if (*info->fmt >= '0' && *info->fmt <= '9')
			while (*info->fmt >= '0' && *info->fmt <= '9')
				info->prec = 10 * info->prec + (*info->fmt++ - '0');
		else if (*info->fmt == '*')
		{
			info->prec = va_arg(info->valist, t_int32);
			++info->fmt;
		}
		info->flags |= FLAG_TRUNCATE;
	}
}

static inline void	get_size_modifier(t_printf_info *info)
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
			info->flags |= SIZE_LONG_DBL;
		else if (*info->fmt == 't')
			info->flags |= SIZE_PTR;
		else
			break ;
		++info->fmt;
	}
}

static inline void	print_arg_by_type(t_printf_info *info)
{
	if (*info->fmt == 'B' || *info->fmt == 'X')
		info->capitals = 16;
	if (*info->fmt == 'd' || *info->fmt == 'i' || *info->fmt == 'D')
		print_signed_number(info, 10);
	else if (*info->fmt == 's')
		print_string(info, (info->flags & SIZE_LONG
										|| info->flags & SIZE_LLONG));
	else if (*info->fmt == 'u' || *info->fmt == 'U')
		print_unsigned_number(info, 10);
	else if (*info->fmt == 'o' || *info->fmt == 'O')
		print_unsigned_number(info, 8);
	else if (*info->fmt == 'x' || *info->fmt == 'X')
		print_unsigned_number(info, 16);
	else if (*info->fmt == 'b' || *info->fmt == 'B')
		print_unsigned_number(info, 2);
//	else if (*info->fmt == 'f' || *info->fmt == 'F')
//		(info->flags & F_APP_PRECI && !p->precision) ? pf_putnb(p) : pf_putdouble(p);
	else if (*info->fmt == 'c' || *info->fmt == 'C')
		print_char(info, (*info->fmt == 'C'));
	else if (*info->fmt == 'S')
		print_string(info, 1);
//	else if (*info->fmt == 'p')
//		print_pointer_address(p);
//	else if (*info->fmt == 'n')
//		*va_arg(p->ap, int *) = p->len;
//	else if (*info->fmt == 'm')
//		ft_printf_putstr(strerror(errno), p);
//	else if (*info->fmt == '{')
//		color(p);
//	else
//		cs_not_found(p);
}

void				get_formatted_arg(t_printf_info *info)
{
	info->flags = 0;
	info->width = 0;
	info->prec = 6;
	info->capitals = 0;
	while (*info->fmt)
	{
		if (*info->fmt == '#')
			info->flags |= FLAG_ALT_FORM;
		else if (*info->fmt == '-')
			info->flags |= FLAG_LEFT_ALIGN;
		else if (*info->fmt == '+' || *info->fmt == ' ')
			(*info->fmt == '+') ? (info->flags |= FLAG_PLUS_SIGN)
								: (info->flags |= FLAG_BLANK_SIGN);
		else if (*info->fmt == '\'')
			info->flags |= FLAG_GROUP;
		else if (*info->fmt == '0')
			info->flags |= FLAG_ZERO_PAD;
		else
			break ;
		info->fmt++;
	}
	get_width_n_precision(info);
	get_size_modifier(info);
	print_arg_by_type(info);
}
