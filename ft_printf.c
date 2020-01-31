#include "ft_printf.h"

static inline int	entry(register t_printf_info *info)
{
	register size_t		non_format_chars;
	char	 			*non_format_str_beg;

	while (*info->fmt)
	{
		non_format_chars = 0;
		non_format_str_beg = (char*)info->fmt;
		while (*info->fmt && *info->fmt != '%')
		{
			non_format_chars++;
			info->fmt++;
		}
		do_print(info, non_format_str_beg, non_format_chars);
		if (!*info->fmt || !*(++info->fmt))
			break ;
		get_formatted_arg(info);
		info->fmt++;
	}
	write(info->fd, info->buff, info->buff_index);
	va_end(info->ap);
	return (info->printed);
}

int 				ft_dprintf(int fd, const char *format, ...)
{
	t_printf_info	info;

	if (fd < 0 || fd > FD_SETSIZE)
		return (-1);
	ft_bzero(&info, sizeof(t_printf_info));
	info.fd = fd;
	info.fmt = format;
	va_start(info.ap, format);
	return (entry(&info));
}

int 				ft_sprintf(char *str, const char *format, ...)
{
	t_printf_info	info;

	ft_bzero(&info, sizeof(t_printf_info));
	info.fd = 1;
	info.fmt = format;
	info.write_to_str = 1;
	info.str_to_write = str;
	va_start(info.ap, format);
	return (entry(&info));
}

int 				ft_printf(const char *format, ...)
{
	t_printf_info	info;

	ft_bzero(&info, sizeof(t_printf_info));
	info.fd = 1;
	info.fmt = format;
	va_start(info.ap, format);
	return (entry(&info));
}
