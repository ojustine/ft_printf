#include "ft_printf.h"

void	do_print_dbl(t_printf_info *info, t_binary64 bin64)
{
	t_fxd_dbl		fxd_dbl;
	t_fxd_dbl		exp;
	t_fxd_img		e;

	if (*info->fmt == 'a' || *info->fmt == 'A')
	{
		//print_unsigned
		return ;
	}
	ft_memset(&fxd_dbl, 0, sizeof(t_fxd_dbl));
	ft_memset(&exp, 0, sizeof(t_fxd_dbl));
	fxd_dbl_build_mantis(bin64, &fxd_dbl);
	fxd_dbl_build_exp(bin64.s_parts.bias_exp, &exp);
	fxd_dbl_mul(&fxd_dbl, &exp);
	e.int_len = fxd_dbl.int_len;
	e.frc_len = fxd_dbl.frc_len;
	e.val = fxd_dbl.val;
	if (*info->fmt == 'f' || *info->fmt == 'F')
	{
		print_fp_decimal_form(info, &fxd_dbl);
		return ;
	}
	else if (*info->fmt == 'e' || *info->fmt == 'E')
	{
		return;
	}
	else
	{

	}
}

void	get_floating_point_arg(t_printf_info *info)
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

void test()
{
	t_printf_info p;
	t_binary64 b;

	p.prec = 12;
	p.fmt = "f";
	b.val = 3.14;
	do_print_dbl(&p, b);
}
