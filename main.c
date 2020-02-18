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
//TODO ("{%*3d}", 5, 0) ("%.c", 0) ("%p", 0) ("{%5p}", 0) ("%.0p, %.p", 0, 0)
	//ft_printf("%.c", 48);
	//printf("%.c", 48);
	//printf("%5.0i", 0);
	//printf("% hZ%", 0);
	ft_printf("%Lf\n", LDBL_MAX);
	//printf("%d\n", LDBL_MAX_10_EXP / 9);
	//printf("%.2f\n", 9.999999999999);
	//printf("%d\n", FP_D_LEN * FP_R_LEN);
}//log 10 2 * (log 2 mantissa + binary exp) - machine epsilon)