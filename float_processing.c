#include "ft_printf.h"

void	fxd_del(t_fxd *fp1, t_fxd *fp2)
{
	if (fp1 != NULL)
	{
		if (fp1->val != NULL)
			free(fp1->val);
		free(fp1);
	}
	if (fp2 != NULL)
	{
		if (fp2->val != NULL)
			free(fp2->val);
		free(fp2);
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
		if (frac_size > FP_LD_LEN - FP_LD_POINT)
			frac_size = FP_LD_LEN - FP_LD_POINT;
		fxd_new->val = malloc(FP_R_SIZE * (FP_LD_POINT + frac_size + 1));
		ft_assert(fxd_new->val != NULL, __FUNCTION__, "malloc error");
		ft_bzero(fxd_new->val, FP_R_SIZE * (FP_LD_POINT + frac_size + 1));
		fxd_new->f0 = FP_LD_POINT + 1;
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
		int_fast16_t bias_exp, char sign)
{
	t_fxd		*mantis;
	t_fxd		*fp;
	size_t		to_print;
	char		buff[FP_D_CHAR_LEN + 9];

	info->prec = (info->prec > FP_D_MAX_PREC) ? FP_D_MAX_PREC : info->prec;
	bias_exp = (bias_exp != 0) ? bias_exp - 1023 : -1022;
	if (*info->fmt == 'a' || *info->fmt == 'A')
		to_print = ldtoa_hex_form(info, bin_mantis, bias_exp, buff);
	else
	{
		mantis = fxd_build_mantis(bin_mantis, bias_exp != 0, 0);
		fp = fxd_get_pow_2(bias_exp, 0);
		fxd_dbl_mul(fp, fp, mantis, 0);
		if (*info->fmt == 'f' || *info->fmt == 'F')
			to_print = fxd_ftoa_dec_form(info, fp, buff);
		else if (*info->fmt == 'e' || *info->fmt == 'E')
			to_print = fxd_ftoa_exp_form(info, fp, buff);
		else
			to_print = fxd_ftoa_opt_form(info, fp, buff);
		fxd_del(fp, mantis);
	}
	info->width -= set_prefix_fp(info, sign, to_print);
	do_print(info, buff, to_print);
	padding(info, info->width, ' ');
}

void	do_print_ldbl(t_printf_info *info, uint_fast64_t bin_mantis,
		int_fast32_t bias_exp, char sign)
{
	t_fxd		*mantis;
	t_fxd		*fp;
	size_t		to_print;
	char		buff[FP_LD_CHAR_LEN];

	info->prec = (info->prec > FP_LD_MAX_PREC) ? FP_LD_MAX_PREC : info->prec;
	bias_exp = (bin_mantis & FP_LD_64BIT) ? bias_exp - 16382 : -16381;
	if (*info->fmt == 'a' || *info->fmt == 'A')
		to_print = ldtoa_hex_form(info, bin_mantis, bias_exp, buff);
	else
	{
		mantis = fxd_build_mantis(bin_mantis, bin_mantis & FP_LD_64BIT, 1);
		fp = fxd_get_pow_2(bias_exp, 1);
		fxd_dbl_mul(fp, fp, mantis, 1);
		if (*info->fmt == 'f' || *info->fmt == 'F')
			to_print = fxd_ftoa_dec_form(info, fp, buff);
		else if (*info->fmt == 'e' || *info->fmt == 'E')
			to_print = fxd_ftoa_exp_form(info, fp, buff);
		else
			to_print = fxd_ftoa_opt_form(info, fp, buff);
		fxd_del(fp, mantis);
	}
	info->width -= set_prefix_fp(info, sign, to_print);
	do_print(info, buff, to_print);
	padding(info, info->width, ' ');
}

void	get_floating_point_arg(t_printf_info *info)
{
	t_binary64			b64;
	t_binary80			b80;
	const int_fast16_t	fast = (info->prec < 10
						&& (*info->fmt == 'f' || *info->fmt == 'F')
						&& (info->prec > 0 || !(info->flags & FLAG_ALT_FORM)));

	if (info->flags & SIZE_LDBL && IS_LONG_DBL_DEFINED)
	{
		b80.val = va_arg(info->ap, long double);
		if (b80.s_pts.exp == 0x7FFF)
			fxd_ftoa_inf_nan(info, b64.s_pts.mant, b64.s_pts.sign);
		else if (fast && (info->prec + ft_longlen(b80.val) <= LDBL_DIG))
			fast_dtoa(info, (double)b80.val);
		else
			do_print_ldbl(info, b80.s_pts.mant, b80.s_pts.exp, b80.s_pts.sign);
		return ;
	}
	b64.val = va_arg(info->ap, double);
	if (b64.s_pts.exp == 0x7FF)
		fxd_ftoa_inf_nan(info, b64.s_pts.mant, b64.s_pts.sign);
	else if (fast && (info->prec + ft_longlen(b64.val) <= DBL_DIG))
		fast_dtoa(info, b64.val);
	else
		do_print_dbl(info, b64.s_pts.mant, b64.s_pts.exp, b64.s_pts.sign);
}
