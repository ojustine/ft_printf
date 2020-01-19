#include "ft_printf.h"

static inline void	fxd_point_add_frac_pow_2(t_fxd_point_dbl *fxd_point_dbl,
					int_fast16_t index)
{
	int_fast16_t	i;
	int_fast32_t	carry;

	i = 7;
	carry = 0;
	while (i >= 0)
	{
		fxd_point_dbl->frac[i] += g_neg_powers_2[index][i];
		carry = fxd_point_dbl->frac[i] / 1000000000;
		fxd_point_dbl->frac[i] %= 1000000000;
		i--;
	}
	if (carry)
		fxd_point_dbl->ints[0] += carry;
}

void				fxd_point_build_mantis(t_binary64 bin64,
					t_fxd_point_dbl *fxd_point_dbl)
{
	register int_fast16_t	i;

	i = 51;
	while (bin64.s_parts.mantis != 0)
	{
		if (bin64.s_parts.mantis & 1U)
			fxd_point_add_frac_pow_2(fxd_point_dbl, i);
		bin64.s_parts.mantis >>= 1U;
		i--;
	}
}
