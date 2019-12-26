#include "ft_printf.h"

int32_t ft_ceil(double val)
{
	int32_t num;

	num = (int32_t)val;
	return (((double)num == val) ? num : num + 1);
}

float my_ceil(float f)
{
	unsigned input;
	memcpy(&input, &f, 4);
	int exponent = ((input >> 23) & 255) - 127;
	if (exponent < 0) return (f > 0);

	int fractional_bits = 23 - exponent;
	if (fractional_bits <= 0) return f;

	unsigned integral_mask = 0xffffffff << fractional_bits;
	unsigned output = input & integral_mask;

	memcpy(&f, &output, 4);
	if (f > 0 && output != input) ++f;

	return f;
}
