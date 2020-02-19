#include <stdio.h>
#include "ft_printf.h"

int 				ft_printf(const char *format, ...);

int main()
{
	int c;
	int d;
	char str[20];
	wchar_t *nn = L"ǑabcǑ\0";
	wchar_t *n = L"фбв\0";
	//ft_printf("%.c", 48);
	//printf("%.c", 48);
	//printf("%5.0i", 0);
	//printf("% hZ%", 0);
	ft_printf("%m \033[01m 0123 \033[00m 123");
	int y = printf("%.s\n", 0);
	y = ft_printf("абв", 0);
	//printf("%d\n", LDBL_MAX_10_EXP / 9);
	//printf("%.2f\n", 9.999999999999);
	//printf("%d\n", FP_D_LEN * FP_R_LEN);
}//log 10 2 * (log 2 mantissa + binary exp) - machine epsilon)