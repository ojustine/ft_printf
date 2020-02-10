#include "ft_printf.h"

void	fxd_del(t_fxd *fp1, t_fxd *fp2, t_fxd *fp3)
{
	if (fp1 != NULL)
	{
		if (fp1->val != NULL)
			free(fp1->val);
		free(fp1);
		fp1 = NULL;
	}
	if (fp2 != NULL)
	{
		if (fp2->val != NULL)
			free(fp2->val);
		free(fp2);
		fp2 = NULL;
	}
	if (fp3 != NULL)
	{
		if (fp3->val != NULL)
			free(fp3->val);
		free(fp3);
		fp3 = NULL;
	}
}

t_fxd	*fxd_new(size_t frac_size, int_fast16_t is_long_dbl)
{
	t_fxd *fxd_new;

	fxd_new = malloc(sizeof(t_fxd));
	ft_assert(fxd_new != NULL, __FUNCTION__, "malloc error");
	ft_bzero(fxd_new, sizeof(t_fxd));
	if (is_long_dbl)
	{
		if (frac_size > LD_LEN - LD_POINT)
			frac_size = LD_LEN - LD_POINT;
		fxd_new->val = malloc(FP_R_SIZE * (LD_POINT + frac_size + 1));
		ft_assert(fxd_new->val != NULL, __FUNCTION__, "malloc error");
		ft_bzero(fxd_new->val, FP_R_SIZE * (LD_POINT + frac_size + 1));
		fxd_new->f0 = LD_POINT + 1;
	}
	else
	{
		if (frac_size > FP_D_LEN - FP_D_POINT)
			frac_size = FP_D_LEN - FP_D_POINT;
		fxd_new->val = malloc(FP_R_SIZE * (FP_D_POINT + frac_size + 1));
		ft_assert(fxd_new->val != NULL, __FUNCTION__, "malloc error");
		ft_bzero(fxd_new->val, FP_R_SIZE * (FP_D_POINT + frac_size + 1));
		fxd_new->f0 = FP_D_POINT + 1;
	}
	return (fxd_new);
}

void	do_print_dbl(t_printf_info *info, uint_fast64_t bin_mantis,
		uint_fast16_t bias_exp, uint_fast16_t sign)
{
	t_fxd		*mantis;
	t_fxd		*exp;
	t_fxd		*fp;
	char		buff[FP_D_CHAR_LEN + 1];
	size_t		to_print;

	info->prec = (info->prec > FP_D_MAX_PREC) ? FP_D_MAX_PREC : info->prec;
	if (*info->fmt == 'a' || *info->fmt == 'A')
		return;//TODO: print_hex
	else
	{
		mantis = fxd_build_mantiss(bin_mantis, bias_exp != 0, 0);
		exp = fxd_get_pow_2(bias_exp - 1023, 0);
		fp = fxd_new(mantis->frc_len + exp->frc_len, 0);
		fxd_dbl_mul(fp, mantis, exp, 0);
		if (*info->fmt == 'f' || *info->fmt == 'F')
			to_print = fxd_ftoa_dec_form(info, fp, buff);
		if (*info->fmt == 'e' || *info->fmt == 'E')
			to_print = fxd_ftoa_exp_form(info, fp, buff);
	}
	do_print(info, buff, to_print);
	fxd_del(fp, mantis, exp);
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
		do_print_dbl(info, bin64.s_parts.mantis, bin64.s_parts.bias_exp, bin64.s_parts.sign);
	}
}
