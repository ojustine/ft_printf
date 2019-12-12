#include "ft_printf.h"
#include <stdio.h>

struct hp_t {
	double val, off;
};

typedef union {
	double d;
	uint64_t i;
} errol_bits_t;

static inline void hp_normalize(struct hp_t *hp)
{
	double val;
	val = hp->val;
	hp->val += hp->off;
	hp->off += val - hp->val;
}

static inline void dbl_mul_div_10(struct hp_t *hp, int16_t is_mul)
{
	double val;
	double off;
	val = hp->val;

	if (is_mul)
	{
		hp->val *= 10.0;
		hp->off *= 10.0;
		off = hp->val;
		off -= val * 8.0;
		off -= val * 2.0;
		hp->off -= off;
	}
	else
	{
		hp->val /= 10.0;
		hp->off /= 10.0;
		val -= hp->val * 8.0;
		val -= hp->val * 2.0;
		hp->off += val / 10.0;
	}
	hp_normalize(hp);
}

static inline double fpnext(double val)
{
	errol_bits_t bits = { val };
	bits.i++;
	return bits.d;
}

static inline double fpprev(double val)
{
	errol_bits_t bits = { val };
	bits.i--;
	return bits.d;
}

int errol0_dtoa(double val, char *buf)
{
	double ten;
	int exp;
	struct hp_t mid, high, low;

	if(val == DBL_MAX) {
		strcpy(buf, "17976931348623157");
		return 309;
	}
	ten = 1.0;
	exp = 1;
	mid.val = val;
	mid.off = 0.0;
	while(((mid.val > 10.0) || ((mid.val == 10.0) && (mid.off >= 0.0))) && (exp < 308))
		exp++, dbl_mul_div_10(&mid, 0), ten /= 10.0;
	while(((mid.val < 1.0) || ((mid.val == 1.0) && (mid.off < 0.0))) && (exp > -307))
		exp--, dbl_mul_div_10(&mid, 1), ten *= 10.0;
	high.val = mid.val;
	high.off = mid.off + (fpnext(val) - val) * ten / (2.0 + 0.0000001);
	low.val = mid.val;
	low.off = mid.off + (fpprev(val) - val) * ten / (2.0 + 0.0000001);
	hp_normalize(&high);
	hp_normalize(&low);
	while(high.val > 10.0 || (high.val == 10.0 && (high.off >= 0.0)))
		exp++, dbl_mul_div_10(&high, 0), dbl_mul_div_10(&low, 0);
	while(high.val < 1.0 || (high.val == 1.0 && (high.off < 0.0)))
		exp--, dbl_mul_div_10(&high, 1), dbl_mul_div_10(&low, 1);
	while(high.val != 0.0 || high.off != 0.0) {
		uint8_t ldig, hdig = 0;
		hdig = (uint8_t)(high.val);
		if((high.val == hdig) && (high.off < 0))
			hdig -= 1;
		ldig = (uint8_t)(low.val);
		if((low.val == ldig) && (low.off < 0))
			ldig -= 1;
		if(ldig != hdig)
			break;
		*buf++ = hdig + '0';
		high.val -= hdig;
		dbl_mul_div_10(&high, 1);
		low.val -= ldig;
		dbl_mul_div_10(&low, 1);
	}

	double mdig = (high.val + low.val) / 2.0 + 0.5;
	*buf++ = (uint8_t)(mdig) + '0';
	*buf = '\0';

	return exp;
}