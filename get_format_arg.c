#include "ft_printf.h"

#define IS_FMT(FMT) (*info->fmt == (FMT))
#define SET_FLAG(FLAG) (info->flags |= FLAG)
#define RM_FLAG(FLAG) (info->flags &= ~FLAG)
#define IS_FLAG(FLAG) (info->flags & FLAG)

static inline void	get_width_n_precision(t_printf_info *info)
{
	if (*info->fmt >= '0' && *info->fmt <= '9')
		while (*info->fmt >= '0' && *info->fmt <= '9')
			info->width = 10 * info->width + (*info->fmt++ - '0');//TODO width < 0 is left-flag
	else if (*info->fmt == '*')
	{
		info->width = va_arg(info->ap, int32_t);
		++info->fmt;
		if (info->width < 0 && (info->width = -info->width))//TODO what happ if prec < 0?
			SET_FLAG(FLAG_LEFT_ALIGN);
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
			info->prec = va_arg(info->ap, int32_t);
			++info->fmt;
		}
		SET_FLAG(FLAG_TRUNCATE);
	}
}

static inline void	get_size_modifier(t_printf_info *info)
{
	while (*info->fmt)
	{
		if (IS_FMT('z'))
			SET_FLAG(SIZE_SIZE_T);
		else if (*info->fmt == 'l')
			SET_FLAG((*(info->fmt + 1) == 'l' && ++info->fmt)
			? SIZE_LLONG : SIZE_LONG);
		else if (*info->fmt == 'h')
			SET_FLAG((*(info->fmt + 1) == 'h' && ++info->fmt)
			? SIZE_CHAR : SIZE_SHORT);
		else if (*info->fmt == 'j')
			SET_FLAG(SIZE_INTMAX);
		else if (*info->fmt == 'L')
			SET_FLAG(SIZE_LDBL);
		else if (*info->fmt == 't')
			SET_FLAG(SIZE_PTR);
		else
			break ;
		++info->fmt;
	}
}

static inline void	print_arg_by_type(t_printf_info *info)
{
	if (IS_FMT('B') || IS_FMT('X'))
		info->cap = 1;//TODO strany, cap == 1 || cap == 0
	if (IS_FMT('d') || IS_FMT('i') || IS_FMT('D'))
		get_signed_arg(info, 10);
	else if (*info->fmt == 's')
		get_string_arg(info, (info->flags & SIZE_LONG
							  || info->flags & SIZE_LLONG));
	else if (*info->fmt == 'u' || *info->fmt == 'U')
		get_unsigned_arg(info, 10);
	else if (*info->fmt == 'o' || *info->fmt == 'O')
		get_unsigned_arg(info, 8);
	else if (*info->fmt == 'x' || *info->fmt == 'X')
		get_unsigned_arg(info, 16);
	else if (*info->fmt == 'b' || *info->fmt == 'B')
		get_unsigned_arg(info, 2);
	else if (*info->fmt == 'f' || *info->fmt == 'F' || *info->fmt == 'e' || *info->fmt == 'E' || *info->fmt == 'g' || *info->fmt == 'G')
		get_floating_point_arg(info);
	else if (*info->fmt == 'c' || *info->fmt == 'C')
		get_char_arg(info, (*info->fmt == 'C'));
	else if (*info->fmt == 'S')
		get_string_arg(info, 1);
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
	info->cap = 0;
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
