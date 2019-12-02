#include "ft_printf.h"

static int	do_print(t_format_spec *info)
{
	while (*info->format)
	{
		if (*info->format != '%')
		{
//			buffer_n_print();
			info->format++;
			continue ;
		}
	}
}

int 		ft_dprintf(int fd, const char *format, ...)
{
	t_format_spec	info;

	if (fd < 0 || fd > FD_SETSIZE)
		return (-1);
	ft_memset(&info, 0, sizeof(t_format_spec));
	info.fd = fd;
	info.format = format;
	va_start(info.valist, format);
	return (do_print(&info));
}

int 		ft_sprintf(char *str, const char *format, ...)
{
	t_format_spec	info;

	ft_memset(&info, 0, sizeof(t_format_spec));
	info.fd = 1;
	info.format = format;
	info.str_to_write = str;
	va_start(info.valist, format);
	return (do_print(&info));
}

int 		ft_printf(const char *format, ...)
{
	t_format_spec	info;

	ft_memset(&info, 0, sizeof(t_format_spec));
	info.fd = 1;
	info.format = format;
	va_start(info.valist, format);
	return (do_print(&info));
}
