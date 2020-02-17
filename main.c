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
	printf("%x\n", 123456);
	ft_printf("%llx\n", UINT64_MAX);
	//size_t i = ft_ultoa_hex(UINT64_MAX, str, 1);
	//ft_printf("%13Lg\n", -LDBL_TRUE_MIN);
	//printf("%13Lg\n", -LDBL_TRUE_MIN);
	//printf("%Lf\n", LDBL_MAX);
	//printf("%d\n", LDBL_MAX_10_EXP / 9);

	//printf("%.2f\n", 9.999999999999);
	//printf("%d\n", FP_D_LEN * FP_R_LEN);
}//log 10 2 * (log 2 mantissa + binary exp) - machine epsilon)