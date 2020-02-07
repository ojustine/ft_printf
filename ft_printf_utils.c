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


static inline void		padding(t_printf_info *info)
{
	const char	zero_pad[] = "0000";
	const char	blank_pad[] = "    ";
	char		*curr_pad;

	if (info->width > 0)
	{
		curr_pad = (char *)((info->flags & FLAG_ZERO_PAD
							 && !(info->flags & FLAG_TRUNCATE)
							 && !(info->flags & FLAG_LEFT_ALIGN))
							? zero_pad : blank_pad);
		while (info->width >= 4)
		{
			do_print(info, curr_pad, 4);
			info->width -= 4;
		}
		while (info->width--)
			do_print(info, curr_pad, 1);
	}
}
