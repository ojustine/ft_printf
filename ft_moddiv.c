/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_moddiv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 11:23:56 by slynell           #+#    #+#             */
/*   Updated: 2020/02/17 11:24:03 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int32_t		ft_moddiv(int32_t dividend, int32_t divisor, int32_t *quotient)
{
	*quotient = dividend / divisor;
	return (dividend % divisor);
}
