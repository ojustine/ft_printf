#include "ft_printf.h"

static inline void	get_width_n_precision(t_printf_info *info)
{
	if (*info->format >= '0' && *info->format <= '9')
		while (*info->format >= '0' && *info->format <= '9')
			info->width = 10 * info->width + (*info->format++ - '0');
	else if (*info->format == '*')
	{
		info->width = va_arg(info->valist, t_int32);
		++info->format;
		if (info->width < 0 && (info->width = -info->width))
			info->flags &= FLAG_LEFT_ALIGN;
	}
	if (*info->format == '.')
	{
		++info->format;
		if (*info->format >= '0' && *info->format <= '9')
			while (*info->format >= '0' && *info->format <= '9')
				info->prec = 10 * info->prec + (*info->format++ - '0');
		else if (*info->format == '*')
		{
			info->prec = va_arg(info->valist, t_int32);
			++info->format;
		}
		else
			info->prec = 0;
		info->flags |= FLAG_TRUNCATE;
	}
}

static inline void	get_size_modifier(t_printf_info *info)
{
	while (*info->format)
	{
		if (*info->format == 'z')
			info->flags |= SIZE_SIZE_T;
		else if (*info->format == 'l')
			info->flags |= (*(info->format + 1) == 'l' && ++info->format)
			? SIZE_LLONG : SIZE_LONG;
		else if (*info->format == 'h')
			info->flags |= (*(info->format + 1) == 'h' && ++info->format)
			? SIZE_CHAR : SIZE_SHORT;
		else if (*info->format == 'j')
			info->flags |= SIZE_INTMAX;
		else if (*info->format == 'L')
			info->flags |= SIZE_LONG_DBL;
		else if (*info->format == 't')
			info->flags |= SIZE_PTR;
		else
			break ;
		++info->format;
	}
}

static inline void	print_arg_by_type(t_printf_info *info)
{
//	if (*info->format == 's')
//		(info->flags & F_LONG || info->flags & F_LONG2) ? pf_putwstr(p) : pf_putstr(p);
//	else if (ft_strchr("dDi", *info->format))
//		pf_putnb(p);
//	else if (*info->format == 'f' || *info->format == 'F')
//		(info->flags & F_APP_PRECI && !p->precision) ? pf_putnb(p) : pf_putdouble(p);
//	else if (*info->format == 'c' || *info->format == 'C')
//		(*info->format == 'c') ? (put_arg_into_buffer(info, *info->format, 1))
//								: print_wchar(info, *info->format);
//	else if (ft_strchr("oOuUbBxX", *info->format))
//		pf_putnb_base(ft_strchri_lu(".b..ou..x", *info->format, -1) << 1, p);
//	else if (*info->format == 'S')
//		pf_putwstr(p);
//	else if (*info->format == 'p')
//		print_pointer_address(p);
//	else if (*info->format == 'n')
//		*va_arg(p->ap, int *) = p->len;
//	else if (*info->format == 'm')
//		ft_printf_putstr(strerror(errno), p);
//	else if (*info->format == '{')
//		color(p);
//	else
//		cs_not_found(p);
}

void				print_format_arg(t_printf_info *info)
{
	info->flags = 0;
	info->width = 0;
	info->prec = 6;
	while (*info->format)
	{
		if (*info->format == '#')
			info->flags |= FLAG_ALT_FORM;
		else if (*info->format == '-')
			info->flags |= FLAG_LEFT_ALIGN;
		else if (*info->format == '+' || *info->format == ' ')
			(*info->format == '+') ? (info->flags |= FLAG_PLUS_SIGN)
									: (info->flags |= FLAG_BLANK_SIGN);
		else if (*info->format == '\'')
			info->flags |= FLAG_GROUP;
		else if (*info->format == '0')
			info->flags |= FLAG_ZERO_PAD;
		else
			break ;
		info->format++;
	}
	get_width_n_precision(info);
	get_size_modifier(info);
}
