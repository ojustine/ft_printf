#include <stdio.h>
#include "ft_printf.h"

int 				ft_printf(const char *format, ...);
void test();

int main()
{
	_Bool b = -1;
	int c;
	char dst[20];
	char *src = "";
	wchar_t *nn = L"ǑabcǑ\0";
	t_float80 l = (t_float80)1.7e+308;

	l *= l;
	l *= l;

	//printf("%LG\n", l);
	printf("%u\n", MB_LEN_MAX);
	printf("%-10.s\n", src);
	c = ft_printf("%#09.6llx\n", LLONG_MAX);
	printf("%#09.6llx\n", LLONG_MAX);
	printf("%i\n", c);

	char buff[100];
	//int r = errol0_dtoa(0.34125159818571591295293549719509501958915810514058, buff);
	//printf("%s %d %.78lf\n", buff, r, 0.34125159818571591295293549719509501958915810514058);

	float f = 3.14f;
	int bbb = *((int*)&f);
	printf("%d\n", bbb);
	test();
}