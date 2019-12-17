#include "ft_printf.h"
#include <stdio.h>

uint32_t LogBase2(uint32_t val)
{
	static const uint8_t logTable[256] =
			{
					0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
					4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
					6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
					6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
					6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
					6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
					7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
					7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
					7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
					7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
					7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
					7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
					7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
					7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
			};

	uint32_t temp;
	temp = val >> 24U;
	if (temp)
		return 24 + logTable[temp];
	temp = val >> 16U;
	if (temp)
		return 16 + logTable[temp];
	temp = val >> 8U;
	if (temp)
		return 8 + logTable[temp];
	return logTable[val];
}

int32_t ft_ceil(double val)
{
	int32_t num = (int32_t)val;
	return (((double)num == val) ? num : num + 1);
}

t_float32	get_mantissa(uint32_t bin_mantissa)
{
	int32_t		i;
	t_float32	res;

	i = 0;
	res = 0.0;
	while (bin_mantissa != 0)
	{
		if (bin_mantissa & 0x400000U)
			res += frac_powers_2[i];
		bin_mantissa <<= 1U;
		i++;
	}
}

void test()
{
	const double log10_2 = 0.30102999566398119521373889472449;

	float val = 3.14f;
	int num = *((int*)&val);
	int mant = (num & 8388607);
	int exponent = ((num >> 23) & 255) - 127;
	int mantx = LogBase2(mant);
	int32_t digitExponent = (ft_ceil((double)(mantx + exponent) * log10_2 - 0.69));
	printf("m%d e%d mx%d ee%d\n", mant, exponent, mantx, digitExponent);
	char buf[1000];
	//exponent = ftoaEngine(val, buf, 10);
	printf("%s %d\n", buf, exponent);
	float n = (1.0f + 0.5f + 0.0625f + 0.00390625f + 0.001953125f + 0.0009765625f + 0.00048828125f + 0.0001220703125f + 0.000030517578125f + 0.0000152587890625f + 0.00000762939453125f + 0.0000002384185791015625f + 0.00000011920928955078125f) * 2;
	printf("%.25f", n);
}