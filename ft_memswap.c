/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memswap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 11:23:44 by slynell           #+#    #+#             */
/*   Updated: 2020/02/17 12:10:18 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static inline void		swap_8(uint8_t *m1, uint8_t *m2)
{
	register uint8_t	tmp;

	tmp = *m1;
	*m1 = *m2;
	*m2 = tmp;
}

static inline void		swap_64(uint8_t *m1, uint8_t *m2)
{
	register uint64_t	tmp;

	tmp = *((uint64_t*)m1);
	*((uint64_t*)m1) = *((uint64_t*)m2);
	*((uint64_t*)m2) = tmp;
}

void					ft_memswap(void *mem1, void *mem2, size_t size)
{
	uint8_t				*m1;
	uint8_t				*m2;

	if (mem1 == NULL || mem2 == NULL || size == 0)
		return ;
	m1 = (uint8_t*)mem1;
	m2 = (uint8_t*)mem2;
	while (size >= 8)
	{
		swap_64(m1, m2);
		m1 += 8;
		m2 += 8;
		size -= 8;
	}
	while (size >= 1)
	{
		swap_8(m1, m2);
		m1++;
		m2++;
		size--;
	}
}
