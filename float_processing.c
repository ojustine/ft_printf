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

int ftoaEngine(float value, char *buffer, int presc)
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
	uint32_t uvalue = *((uint32_t*)(&value));
	uint16_t uvalue_hi16 = (uint16_t)(uvalue >> 16);
	uint8_t exponent = ((uint8_t)(uvalue_hi16 >> 7));
	uint32_t fraction = (uvalue & 0x00ffffff) | 0x00800000;
	char *ptr = buffer;
	if(uvalue & 0x80000000)
		*ptr++ = '-';
	else
		*ptr++ = '+';
	if(exponent == 0) // don't care about a subnormals
	{
		ptr[0] = '0';
		ptr[1] = 0;
		return 0xff;
	}
	if(exponent == 0xff)
	{
		if(fraction & 0x007fffff)
		{
			ptr[0] = 'n';
			ptr[1] = 'a';
			ptr[2] = 'n';
			ptr[3] = 0;
		}
		else
		{
			ptr[0] = 'i';
			ptr[1] = 'n';
			ptr[2] = 'f';
			ptr[3] = 0;
		}
		return 0xff;
	}
	//*ptr++ = '0';
	int exp10 = ((((exponent>>3))*77+63)>>5) - 38;
	fraction <<= 8;
	uint32_t t = (fraction * logTable[exponent]) + 1;
	uint8_t digit = t >> 24;
	digit >>= 4;
	while(digit == 0)
	{
		t &= 0x0fffffff;
		t <<= 1;
		t += t << 2;
		digit = (uint8_t)(t >> 24);
		digit >>= 4;
		exp10--;
	}
	for(uint_fast8_t i = presc+1; i > 0; i--)
	{
		digit = (uint8_t)(t >> 24);
		digit >>= 4;
		*ptr++ = digit + '0';
		t &= 0x0fffffff;
		t <<= 1;
		t += t << 2;
	}
	if(buffer[presc+2] >= '5')
		buffer[presc+1]++;
	ptr[-1] = 0;
	ptr-=2;
	for(uint_fast8_t i = presc + 1; i > 1; i--)
	{
		if(buffer[i] > '9')
		{
			buffer[i]-=10;
			buffer[i-1]++;
		}
	}
	while(ptr[0] == '0')
	{
		*ptr-- = 0;
	}
	return exp10;
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
	exponent = ftoaEngine(val, buf, 10);
	printf("%s %d\n", buf, exponent);
	float n = (1.0f + 0.5f + 0.0625f + 0.00390625f + 0.001953125f + 0.0009765625f + 0.00048828125f + 0.0001220703125f + 0.000030517578125f + 0.0000152587890625f + 0.00000762939453125f + 0.0000002384185791015625f + 0.00000011920928955078125f) * 2;
	printf("%.25f", n);
}