#include "ft_printf.h"

void	fxd_del(t_fxd *fp1, t_fxd *fp2, t_fxd *fp3)
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
	if (fp3 != NULL)
	{
		if (fp3->val != NULL)
			free(fp3->val);
		free(fp3);
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
		uint_fast16_t bias_exp, char sign)
{
	t_fxd		*mantis;
	t_fxd		*fp;
	char		buff[FP_D_CHAR_LEN + 9];
	size_t		to_print;

	info->prec = (info->prec > FP_D_MAX_PREC) ? FP_D_MAX_PREC : info->prec;
	if (*info->fmt == 'a' || *info->fmt == 'A')
		return;//TODO: print_hex
	else
	{
		mantis = fxd_build_mantis(bin_mantis, bias_exp != 0, 0);
		fp = fxd_get_pow_2(bias_exp - 1023, 0);
		fxd_dbl_mul(fp, fp, mantis, 0);
		if (*info->fmt == 'e' || *info->fmt == 'E')
			to_print = fxd_ftoa_exp_form(info, fp, buff);
		if (*info->fmt == 'f' || *info->fmt == 'F')
			to_print = fxd_ftoa_dec_form(info, fp, buff);
	}
	//info->width -= add_prefix;
	do_print(info, buff, to_print);
	//add_postfix
	fxd_del(fp, mantis, 0);
}

void	get_floating_point_arg(t_printf_info *info)
{
	t_binary64	b64;
	t_binary80	b80;

	if (info->flags & SIZE_LDBL && IS_LONG_DBL_DEFINED)
	{
		b80.val = va_arg(info->ap, long double);
//		if (b64.s_pts.b_exp == 0x7ff)
//		{
//			fxd_ftoa_inf_nan(info, b64.s_pts.mantis, b64.s_pts.sign, buff);
//			return ;
//		}
	}
	else
	{
		b64.val = va_arg(info->ap, double);
		if (b64.s_pts.b_exp == 0x7ff)
		{
			fxd_ftoa_inf_nan(info, b64.s_pts.mantis, b64.s_pts.sign);
			return ;
		}
		do_print_dbl(info, b64.s_pts.mantis, b64.s_pts.b_exp, b64.s_pts.sign);
	}
//	if (sign)
//		buff[0] = '-';
//	else if (info->flags & FLAG_PLUS_SIGN || info->flags & FLAG_BLANK_SIGN)
//		buff[0] = " +"[info->flags & FLAG_PLUS_SIGN];

}
