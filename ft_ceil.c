/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ceil.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 11:16:31 by slynell           #+#    #+#             */
/*   Updated: 2020/02/17 11:18:47 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libptf.h"

int32_t		ft_ceil(double val)
{
	int32_t	num;

	num = (int32_t)val;
	return (((double)num == val) ? num : num + 1);
}

float		my_ceil(float f)
{
	unsigned	input;
	int			exponent;
	int			fractional_bits;
	unsigned	integral_mask;
	unsigned	output;

	memcpy(&input, &f, 4);
	exponent = ((input >> 23) & 255) - 127;
	if (exponent < 0)
		return (f > 0);
	fractional_bits = 23 - exponent;
	if (fractional_bits <= 0)
		return (f);
	integral_mask = 0xffffffff << fractional_bits;
	output = input & integral_mask;
	memcpy(&f, &output, 4);
	if (f > 0 && output != input)
		++f;
	return (f);
}
