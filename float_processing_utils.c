#include "ft_printf.h"

static inline size_t	fxd_ftoa_dec_form_frac_part(t_printf_info *info, t_fxd *fp, char *buff)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	register int_fast32_t	prec;
	const char				*ptr = buff;

	fxd_roundup(info, fp);
	if (info->prec == 0 && !(info->flags & FLAG_ALT_FORM))
		return (0);
	*buff++ = '.';
	i = -1;
	prec = 0;
	while (++i < fp->frc_len)
	{
		j = FP_R_LEN;
		while ((prec < info->prec) && --j >= 0)
		{
			prec = i * FP_R_LEN + FP_R_LEN - j;
			*buff++ = (char)(fp->val[fp->f0 + i] / FP_R_TOP + '0');
			fp->val[fp->f0 + i] %= FP_R_TOP;
			fp->val[fp->f0 + i] *= 10;
		}
	}
	while (prec++ < info->prec)
		*buff++ = '0';
	return (buff - ptr);
}

size_t					fxd_ftoa_dec_form(t_printf_info *info, t_fxd *fp, char *buff)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	const char				*ptr = buff;

	i = -fp->int_len;
	if (i == 0)
		*buff++ = '0';
	else
	{
		j = ft_intlen(fp->val[fp->f0 + i]);
		fp->val[fp->f0 + i] *= ft_pow(10, FP_R_LEN - j);
		while (i < 0)
		{
			while (--j >= 0)
			{
				*buff++ = (char)(fp->val[fp->f0 + i] / FP_R_TOP + '0');
				fp->val[fp->f0 + i] %= FP_R_TOP;
				fp->val[fp->f0 + i] *= 10;
			}
			j = FP_R_LEN;
			i++;
		}
	}
	return ((buff - ptr) + fxd_ftoa_dec_form_frac_part(info, fp, buff));
}

size_t					fxd_ftoa_exp_form(t_printf_info *info, t_fxd *fp, char *buff)
{
	t_fxd			*mul;
	int_fast32_t	offset;
	int_fast16_t	index;
	const char		*ptr = buff;

	index = -fp->int_len;
	if (fp->int_len == 0 && fp->frc_len > 0)
		while (fp->val[fp->f0 + index] == 0 && index < fp->frc_len)
			index++;
	offset = ft_intlen(fp->val[fp->f0 + index]) - 1;
	mul = fxd_new((index < 0) ? -index : 0, 0);
	mul->frc_len = (index < 0) ? -index : 0;
	mul->int_len = (index > 0) ? index + 1 : 0;//>=
	mul->val[fp->f0 - 1 - index] = ft_pow(10, FP_R_LEN - offset);
	fxd_dbl_mul(fp, fp, mul, 0);
	buff += fxd_ftoa_dec_form(info, fp, buff);
	*buff++ = "eE"[info->capitals];
	*buff++ = "-+"[index <= 0];//<
	//if (index >= 0)
	//	offset = offset - 1;
	if ((offset = ((index >= 0) ? index : -(index + 1)) * FP_R_LEN + offset) < 10)//>
		*buff++ = '0';
	index = ft_intlen(offset = ft_abs(offset));
	while (index-- > 0)
		*buff++ = (char)(ft_divmod(offset, ft_pow(10, index), &offset) + '0');
	fxd_del(mul, 0, 0);
	return (buff - ptr);
}//TODO too many lines ft_abs mb?

size_t					fxd_ftoa_opt_form(t_printf_info *info, t_fxd *fp, char *buff)
{

}
