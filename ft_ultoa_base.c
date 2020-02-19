/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultoa_base.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 14:04:10 by slynell           #+#    #+#             */
/*   Updated: 2020/02/19 14:04:46 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libptf.h"

size_t					ft_ultoa_base(uint64_t value, char *buff,
						const int_fast16_t base, const int_fast16_t is_upper)
{
	static const char	digits[] = "0123456789abcdef0123456789ABCDEF";
	register char		*ptr;
	size_t				ret;

	ptr = buff;
	*ptr++ = digits[(value % base) + (is_upper != 0) * 16];
	value /= base;
	while (value != 0)
	{
		*ptr++ = digits[(value % base) + (is_upper != 0) * 16];
		value /= base;
	}
	ft_strrev(buff, ptr - 1);
	ret = ptr - buff;
	buff[ret] = 0;
	return (ret);
}
