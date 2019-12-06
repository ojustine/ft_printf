#include <stdio.h>
#include "ft_printf.h"

int 				ft_printf(const char *format, ...);

int main()
{
	_Bool b = -1;
	int c;
	t_float80 l = (t_float80)1.7e+308;
	l *= l;
	l *= l;
	//long double ll = 1.7e+4932;
//	t_float80 l = LDBL_MAX;

	//printf("%LG\n", l);
	printf("%.1LG\n", l);
	printf("%lu\n", sizeof(b));
//	ft_memset(ss, 65, 10);
	char dst[20];
	ft_memset(dst, 0, 20);
	char *src = "abcdef12345678910";
	ft_memcpy(dst, src, 14);
	printf("%s\n", __FUNCTION__);
	c = ft_printf("%*abcdef12345678910\n", 10);
	printf("%d %-d %n %d %m\n", -5, c, &c, c);
	char *sss = "(null)";
	printf("%-10s", sss);
}