#include <stdio.h>
#include "ft_printf.h"

int 				ft_printf(const char *format, ...);

int main()
{
	_Bool b = -1;
	int c;
	char dst[20];
	char *src = "abc";
	wchar_t *nn = L"Ǒ\0";
	t_float80 l = (t_float80)1.7e+308;

	l *= l;
	l *= l;

	//printf("%LG\n", l);
	printf("%u\n", MB_LEN_MAX);
	printf("%5.2s\n", src);
	c = ft_printf("%5.2s\n", src);
	printf("%i", c);
}