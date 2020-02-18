#include <stdio.h>
#include "ft_printf.h"

int 				ft_printf(const char *format, ...);

int main()
{
	int c;
	int d;
	char str[20];
	wchar_t *nn = L"ǑabcǑ\0";
	wint_t *n = L"ǑabcǑ\0";

	//c = ft_printf("%S\n", n);
	//printf("%i\n", c);
	//d = printf("%S\n", n);
	//printf("%i\n", d);

	//printf("%010.*f\n", 6, 0.125);
	//ft_printf("%010.*f\n", 6, 0.125);
	//printf("%lli\n", INT16_MIN);
	//ft_printf("%lli\n", INT16_MIN);
	//printf("%#020.25lli\n", INT64_MIN);
	//ft_printf("%#020.25lli\n", INT64_MIN);
	//printf("%13g\n", -DBL_TRUE_MIN);
	//ft_printf("%12.3A\n", 0.1);
	//ft_printf("%.2a\n", 99.99);
	//printf("%.2a\n", 99.99);
	//ft_printf("%12.4e\n", DBL_MIN);
	ft_printf("@moulitest: %5.x %5.0x\n", 0, 0);
	printf("@moulitest: %5.x %5.0x\n", 0, 0);
	ft_printf("%#.6x\n", 0);
	printf("%#.6x\n", 0);
	ft_printf("%#.6u\n", 0);
	printf("%#.6u\n", 0);
	//printf("%Lf\n", LDBL_MAX);
	//printf("%d\n", LDBL_MAX_10_EXP / 9);
	//printf("%.2f\n", 9.999999999999);
	//printf("%d\n", FP_D_LEN * FP_R_LEN);
}//log 10 2 * (log 2 mantissa + binary exp) - machine epsilon)