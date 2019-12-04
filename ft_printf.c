#include <mach/machine.h>
#include "ft_printf.h"

static inline int	do_print(t_printf_info *info)
{
	size_t		non_format_chars;
	const char 	*non_format_str_beg;

	non_format_chars = 0;
	while (*info->format)
	{
		non_format_str_beg = info->format;
		while (*info->format && *info->format != '%')
		{
			non_format_chars++;
			info->format++;
		}
		buffer_n_print(info, (void*)non_format_str_beg, non_format_chars);
		if (!*info->format || !*(++info->format))
			break ;
		print_format_arg(info);
	}
	write(info->fd, info->buff, info->buff_index);
	va_end(info->valist);
	return (info->printed);
}

int 				ft_dprintf(int fd, const char *format, ...)
{
	t_printf_info	info;

	if (fd < 0 || fd > FD_SETSIZE)
		return (-1);
	ft_memset(&info, 0, sizeof(t_printf_info));
	info.fd = fd;
	info.format = format;
	va_start(info.valist, format);
	return (do_print(&info));
}

int 				ft_sprintf(char *str, const char *format, ...)
{
	t_printf_info	info;

	ft_memset(&info, 0, sizeof(t_printf_info));
	info.fd = 1;
	info.format = format;
	info.write_to_str = 1;
	info.str_to_write = str;
	va_start(info.valist, format);
	return (do_print(&info));
}

int 				ft_printf(const char *format, ...)
{
	t_printf_info	info;

	ft_memset(&info, 0, sizeof(t_printf_info));
	info.fd = 1;
	info.format = format;
	va_start(info.valist, format);
	return (do_print(&info));
}
