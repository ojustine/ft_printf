#include "ft_printf.h"

void	do_print_dbl(t_printf_info *info, t_binary64 bin64)
{
	t_fxd_dbl		*mantis;
	t_fxd_dbl		*exp;

	mantis = malloc(sizeof(t_fxd_dbl));
	exp = malloc(sizeof(t_fxd_dbl));
	ft_memset(mantis, 0, sizeof(t_fxd_dbl));
	ft_memset(exp, 0, sizeof(t_fxd_dbl));
	fxd_dbl_build_mantis(bin64, mantis);
	fxd_dbl_build_exp(bin64.s_parts.bias_exp, exp);
	if (*info->fmt == 'a' || *info->fmt == 'A')
	{
		//TODO: print_hex
		return ;
	}
	fxd_dbl_mul(mantis, exp);
	mantis->int_len = (mantis->int_len) ? mantis->int_len : 1;
	info->prec = (info->prec > INT32_MAX) ? INT32_MAX : info->prec;
	if (*info->fmt == 'f' || *info->fmt == 'F')
		print_fp_dec_form(info, mantis->val, mantis->int_len);
	else if (*info->fmt == 'e' || *info->fmt == 'E')
		//TODO: print_exp
		return;
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
