#include "ft_printf.h"

int ft_dprintf(int fd, const char *format, ...)
{
	t_format_spec	info;

	ft_memset(&info, 0, sizeof(t_format_spec));
}

int ft_printf(const char *format, ...)
{
	t_format_spec	info;

	ft_memset(&info, 0, sizeof(t_format_spec));
	info.fd = 1;
	info.format = (char*)format;
	va_start(info.valist, format);
	while (*info.format)
	{
		if (*info.format != '%')
		{
//			buffering();
			info.format++;
			continue ;
		}
	}
}
