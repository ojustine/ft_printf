/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_long_pow.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 11:19:39 by slynell           #+#    #+#             */
/*   Updated: 2020/02/17 11:19:45 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

long double		ft_long_pow(long double base, int_fast16_t power)
{
	if (power == 0)
		return (1);
	if (power < 0)
		return (ft_long_pow(1 / base, -power));
	if (power & 1)
		return (base * ft_long_pow(base, power - 1));
	return (ft_long_pow(base * base, power / 2));
}
