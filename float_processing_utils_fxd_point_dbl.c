#include "ft_printf.h"

void				fxd_point_build_mantis(t_binary64 bin64,
					t_fxd_point_dbl *fxd_point_dbl)
{
	register int_fast16_t	i;
	register int_fast16_t	j;
	int_fast32_t			carry;

	i = 51;
	while (bin64.s_parts.mantis != 0)
	{
		j = 7;
		carry = 0;
		if (bin64.s_parts.mantis & 1U)
			while (j >= 0)
			{
				fxd_point_dbl->frac[j] += (g_neg_powers_2[i][j] + carry);
				carry = fxd_point_dbl->frac[j] / 1000000000;
				fxd_point_dbl->frac[j] %= 1000000000;
				j--;
			}
		bin64.s_parts.mantis >>= 1U;
		i--;
	}
	fxd_point_dbl->ints[0] += (bin64.s_parts.bias_exp != 0);
	fxd_point_dbl->frac_inx = 7;
}

void				fxd_point_build_exp(t_binary64 bin64,
					t_fxd_point_dbl *fxd_point_dbl)
{
	register int_fast16_t	i;

	t_fxd_point_dbl d = {{0}, {0}, 0, 0};
}
