#include "ft_printf.h"

inline int_fast16_t	add_prefix(t_printf_info *info, const int_fast16_t base,
					char* buff)
{
	int_fast16_t	ret;

	ret = 0;
//	if (sign)
//	{
//		do_print(info, &sign, 1);
//		ret++;
//	}
	if (base != 10 && info->flags & FLAG_ALT_FORM)
	{
		if (base == 8)
		{
			do_print(info, "0", 1);
			return (ret + 1);
		}
		else if (base == 2)
			do_print(info, info->capitals ? "0B" : "0b", 2);
		else if (base == 16)
			do_print(info, info->capitals ? "0X" : "0x", 2);
		return (ret + 2);
	}
	return (ret);
}
