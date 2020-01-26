#include "ft_printf.h"
#include <stdio.h>

void	do_print_dbl(t_printf_info *info, t_binary64 bin64)
{
	t_fxd_dbl		fxd_dbl;
	t_fxd_dbl		exp;

	ft_memset(&fxd_dbl, 0, sizeof(t_fxd_dbl));
	ft_memset(&exp, 0, sizeof(t_fxd_dbl));
	fxd_dbl_build_mantis(bin64, &fxd_dbl);
	fxd_dbl_build_exp(bin64.s_parts.bias_exp, &exp);
	fxd_dbl_mul(&fxd_dbl, &exp);
}

void	print_floating_point_number(t_printf_info *info)
{
	t_binary64	bin64;
	t_binary80	bin80;

	if (info->flags & SIZE_LONG_DBL && IS_LONG_DBL_DEFINED)
	{
		bin80.val = va_arg(info->ap, long double);
//		if (bin80.s_parts.bias_exp == 0xffff)
//		{
//			if (bin80.s_parts.mantis != 0)
//
//		}
	}
	else
	{
		bin64.val = va_arg(info->ap, double);
		do_print_dbl(info, bin64);
	}
}
