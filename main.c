#include <stdio.h>
#include <unistd.h>
#include <wchar.h>
#include "ft_printf.h"

int main() {
	_Bool b = 0;
	t_float128 l = LDBL_MAX;
	wchar_t t = '\2';
	char *ss = (char*)malloc(11);
	ss[0] = 'a'; ss[1] = 'b'; ss[2] = 'b'; ss[3] = 'b'; ss[4] = 'b'; ss[5] = 'b'; ss[6] = 'b'; ss[7] = 'b'; ss[8] = 'b'; ss[9] = 'b';
	ss[10] = 0;

//	printf("%LG\n", l);
	printf("%s\n", ss);
	ft_memset(ss, 65, 10);
	printf("%s\n", ss);
	printf("%05d\n", -7);
}