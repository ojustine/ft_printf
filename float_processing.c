#include "ft_printf.h"
#include <stdio.h>

//static inline t_float	get_mantissa(register uint32_t bin_mantissa)
//{
//	register int32_t	i;
//	register t_float	res;
//	const t_float		frac_powers_2[24] =
//		{1.0f, 0.5f, 0.25f, 0.125f, 6.25e-2f, 3.125e-2f, 1.5625e-2f, 7.8125e-3f,
//		3.90625e-3f, 1.953125e-3f, 9.765625e-4f, 4.8828125e-4f,	2.44140625e-4f,
//		1.220703125e-4f, 6.103515625e-5f, 3.0517578125e-5f, 1.52587890625e-5f,
//		7.62939453125e-6f, 3.814697265625e-6f, 1.9073486328125e-6f,
//		9.5367431640625e-7f, 4.76837158203125e-7f, 2.384185791015625e-7f,
//		1.1920928955078125e-7f};
//
//	i = 1;
//	res = frac_powers_2[0];
//	while (bin_mantissa != 0)
//	{
//		if (bin_mantissa & 0x400000U)
//			res += frac_powers_2[i];
//		bin_mantissa <<= 1U;
//		i++;
//	}
//	return (res);
//}

void	do_print_dbl(t_printf_info *info, t_binary64 bin64)
{
	t_fxd_dbl		fxd_dbl;
	t_fxd_dbl		fxd_dbl1;
	t_binary64 bin1;

	ft_memset(&fxd_dbl, 0, sizeof(t_fxd_dbl));
	ft_memset(&fxd_dbl1, 0, sizeof(t_fxd_dbl));
	bin1.val = 1.545452;
	fxd_dbl_build_mantis(bin64, &fxd_dbl);
	fxd_dbl_build_mantis(bin1, &fxd_dbl1);
	fxd_dbl_add(&fxd_dbl, &fxd_dbl);
	fxd_dbl_mult(&fxd_dbl, &fxd_dbl);
}

void	print_floating_point_number(t_printf_info *info)
{
	t_binary64	bin64;
	t_binary80	bin80;

	if (info->flags & SIZE_LONG_DBL && IS_LONG_DBL)
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
//	if (info->flags & FLAG_PLUS_SIGN || info->flags & FLAG_BLANK_SIGN)
//		sign = (info->flags & FLAG_PLUS_SIGN) ? '+' : ' ';
//	else
//		sign = 0;
//	do_print_num(info, num, base, sign);
}

void test()
{
	t_printf_info p;
	t_binary64 bin;

	p.flags = 0;
	bin.val = 1.545452;
	do_print_dbl(&p, bin);
}