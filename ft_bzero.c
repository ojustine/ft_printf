/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 11:16:18 by slynell           #+#    #+#             */
/*   Updated: 2020/02/17 11:16:26 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void					*ft_bzero(void *b, size_t n)
{
	unsigned char		*mem;
	const uint64_t		zero = 0;
	uint64_t			*ptr_64;
	uint32_t			*ptr_32;

	mem = (unsigned char*)b;
	ptr_64 = (uint64_t*)mem;
	while (n >= 8)
	{
		*ptr_64++ &= zero;
		mem += 8;
		n -= 8;
	}
	ptr_32 = (uint32_t*)mem;
	while (n >= 4)
	{
		*ptr_32++ &= zero;
		mem += 4;
		n -= 4;
	}
	while (n--)
		*mem++ = (unsigned char)0;
	return (b);
}
