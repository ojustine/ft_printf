#include "ft_printf.h"

static inline void		roundup(t_printf_info *info, t_fxd *fp)
{
	uint32_t 				j;
	uint32_t 				trim;
	uint64_t				pow;
	register int_fast16_t	i;

	i = info->prec / FP_R_LEN;
	j = i;
	pow = ft_pow(10, (FP_R_LEN - (info->prec % FP_R_LEN)));
	trim = fp->val[D_F0 + i] % pow;
	if (trim > pow / 10 * 5)
		fp->val[D_F0 + i] += pow;
	else if (trim == pow / 10 * 5)
	{
		while (++j < fp->frc_len && fp->val[D_F0 + i] == 0)
			;
		if (j == fp->frc_len)
			fp->val[D_F0 + i] += ((fp->val[D_F0 + i] / pow % 10) & 1) ? pow : 0;
		else
			fp->val[D_F0 + i] += pow;
	}
	while ((pow = (uint64_t)fp->val[D_F0 + i] / FP_R_LIMITER) > 0)
	{
		fp->val[D_F0 + i] %= FP_R_LIMITER;
		fp->val[D_F0 + --i] += pow;
	}
}

static inline size_t	fxd_ftoa_dec_form_frac_part(t_printf_info *info, t_fxd *fp, char *buff)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	register int_fast32_t	prec;
	const char				*ptr = buff;

	roundup(info, fp);
	if (info->prec == 0)
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
			*buff++ = (char)(fp->val[D_F0 + i] / FP_R_TOP + '0');
			fp->val[D_F0 + i] %= FP_R_TOP;
			fp->val[D_F0 + i] *= 10;
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
		j = ft_intlen(fp->val[D_F0 + i]);
		fp->val[D_F0 + i] *= ft_pow(10, FP_R_LEN - j);
		while (i < 0)
		{
			while (--j >= 0)
			{
				*buff++ = (char)(fp->val[D_F0 + i] / FP_R_TOP + '0');
				fp->val[D_F0 + i] %= FP_R_TOP;
				fp->val[D_F0 + i] *= 10;
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
		while (fp->val[D_F0 + index] == 0 && index < fp->frc_len)
			index++;
	offset = ft_intlen(fp->val[D_F0 + index]) - 1;
	mul = fxd_new((index < 0) ? -index : 0, 0);
	mul->frc_len = (index < 0) ? -index : 0;
	mul->int_len = (index >= 0) ? index + 1 : 0;
	mul->val[D_I0 - index] = ft_pow(10, FP_R_LEN - offset);
	fxd_dbl_mul(fp, fp, mul);
	buff += fxd_ftoa_dec_form(info, fp, buff);
	*buff++ = "eE"[info->capitals];
	*buff++ = "-+"[index < 0];
	if (index >= 0)
		offset = FP_R_LEN - offset;
	if ((offset = ((index > 0) ? index : -(index + 1)) * FP_R_LEN + offset) < 10)
		*buff++ = '0';
	index = ft_intlen(offset = ((offset + (offset >> 15)) ^ offset >> 15));
	while (index-- > 0)
		*buff++ = (char)(ft_divmod(offset, ft_pow(10, index), &offset) + '0');
	fxd_del(mul, 0, 0);
	return (buff - ptr);
}//TODO too many lines ft_abs mb?

size_t					fxd_ftoa_opt_form(t_printf_info *info, t_fxd *fp, char *buff)
{

}
