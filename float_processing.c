#include "ft_printf.h"
#include <stdio.h>

static inline t_float	get_mantissa(register uint32_t bin_mantissa)
{
	register int32_t	i;
	register t_float	res;
	const t_float		frac_powers_2[24] =
		{1.0f, 0.5f, 0.25f, 0.125f, 6.25e-2f, 3.125e-2f, 1.5625e-2f, 7.8125e-3f,
		3.90625e-3f, 1.953125e-3f, 9.765625e-4f, 4.8828125e-4f,	2.44140625e-4f,
		1.220703125e-4f, 6.103515625e-5f, 3.0517578125e-5f, 1.52587890625e-5f,
		7.62939453125e-6f, 3.814697265625e-6f, 1.9073486328125e-6f,
		9.5367431640625e-7f, 4.76837158203125e-7f, 2.384185791015625e-7f,
		1.1920928955078125e-7f};

	i = 1;
	res = frac_powers_2[0];
	while (bin_mantissa != 0)
	{
		if (bin_mantissa & 0x400000U)
			res += frac_powers_2[i];
		bin_mantissa <<= 1U;
		i++;
	}
	return (res);
}

void test()
{
	const float log10_2 = 0.30102999566398119521373889472449f;
	float val = 3.14f;
	t_binary32 t;
	t.val = val;
	int mant = t.s_parts.mantis;
	int exponent = t.s_parts.bias_exp - 127;
	float c = ft_pow(2.0, exponent);
	float n = (get_mantissa(mant)) * c;
	printf("%.25f\n", val);
	printf("%.25f\n", n);
	t_big_float a;
	t_big_float b;
	big_float_parse(&a, "10.1", 2);
	big_float_parse(&b, "100.111", 4);
	t_big_float res;
	//big_float_add(&a, &b, &res, 3);
	big_float_multiply(&a, &b, &res, 3);
}