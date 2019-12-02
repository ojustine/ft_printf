#include <stdio.h>
#include <unistd.h>
#include <wchar.h>
#include "ft_printf.h"

int main() {
	_Bool b = 0;
	t_float128 l = LDBL_MAX;
	wchar_t t = '\2';
	long s = INT_MAX + 1;

	printf("%LG\n", l);
	printf("%d\n", INT_MAX);
	printf("%d\n", FD_SETSIZE);
//	ft_memset(ss, 65, 10);
}