#include "ft_printf.h"

void	fxd_del(t_fxd *fp)
{
	if (fp != NULL)
	{
		if (fp->val != NULL)
			free(fp->val);
		free(fp);
		fp = NULL;
	}
}

t_fxd	*fxd_new(size_t size, int_fast16_t is_long_dbl)
{
	t_fxd *fxd_new;

	fxd_new = malloc(sizeof(t_fxd));
	ft_assert(fxd_new != NULL, __FUNCTION__, "malloc error");
	ft_bzero(fxd_new, sizeof(t_fxd));
	if (size == 0)
		return (fxd_new);
	if (is_long_dbl)
	{
		if (size > LD_LEN)
			size = LD_LEN;
	}
	else
		if (size > D_LEN)
			size = D_LEN;
	fxd_new->val = malloc(size);
	ft_assert(fxd_new->val != NULL, __FUNCTION__, "malloc error");
	ft_bzero(fxd_new->val, size);
	return (fxd_new);
}

void	do_print_dbl(t_printf_info *info, t_binary64 bin64)
{
	t_fxd		*mantis;
	t_fxd		*exp;

	exp = fxd_new(D_LEN, 0);
	mantis = fxd_new(D_LEN, 0);
	fxd_dbl_build_mantis(bin64, mantis);
	print_fp_dec_form(info, mantis);

	fxd_dbl_build_exp(bin64.s_parts.bias_exp, exp);
	if (*info->fmt == 'a' || *info->fmt == 'A')
	{
		//TODO: print_hex
		return ;
	}
	fxd_dbl_mul(mantis, exp);
	mantis->int_len = (mantis->int_len) ? mantis->int_len : 1;
	info->prec = (info->prec > D_MAX_PREC) ? D_MAX_PREC : info->prec;
	if (*info->fmt == 'f' || *info->fmt == 'F')
		print_fp_dec_form(info, mantis);
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
