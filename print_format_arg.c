#include "ft_printf.h"

static inline void	get_flags(t_printf_info *info)
{
	while (*info->format)
	{
		info->pad = ' ';
		if (*info->format == '#')
			info->flags &= FLAG_ALT_FORM;
		else if (*info->format == '-')
			info->flags &= FLAG_LEFT_ALIGN;
		else if (*info->format == '+')
			info->flags &= FLAG_PLUS_SIGN;
		else if (*info->format == '\'')
			info->flags &= FLAG_GROUP;
		else if (*info->format == ' ')
		{
			if (!(info->flags & FLAG_PLUS_SIGN))
				info->flags &= FLAG_BLANK_SIGN;
		}
		else if (*info->format == '0')
			info->pad = '0';
		else
			break;
		info->format++;
	}
}

static inline void	get_width_n_precision(t_printf_info *info)
{
	if (*info->format >= '0' && *info->format <= '9')
		while (*info->format >= '0' && *info->format <= '9')
			info->width = 10 * info->width + (*info->format++ - '0');
	else if (*info->format == '*')
	{
		info->width = va_arg(info->valist, t_int32);
		++info->format;
		if (info->width < 0)
		{
			info->flags &= FLAG_LEFT_ALIGN;
			info->width = -info->width;
		}
	}
	if (*info->format == '.')
	{
		++info->format;
		if (*info->format >= '0' && *info->format <= '9')
			while (*info->format >= '0' && *info->format <= '9')
				info->prec = 10 * info->prec + (*info->format++ - '0');
		else if (*info->format == '*') {
			info->prec = va_arg(info->valist, t_int32);
			++info->format;
		}
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

static inline void	get_type(t_printf_info *info)
{

}

void				print_format_arg(t_printf_info *info)
{
	info->flags = 0;
	info->width = 0;
	info->prec = 0;
	get_flags(info);
	get_width_n_precision(info);
	get_size_modifier(info);
}
