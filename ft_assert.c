/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_assert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 11:16:08 by slynell           #+#    #+#             */
/*   Updated: 2020/02/17 11:16:12 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
**	call examples:
**	ft_assert(i < MAX_LEN, __FUNCTION__, "going beyond the array");
**
**	ft_assert(a->length + shift < FLT_MAX_LEN, __FUNCTION__,
**	"too long float or too much shift - out of array");
*/

void	ft_assert(int_fast32_t to_check, const char *func, const char *message)
{
	if (!to_check)
	{
		write(2, "ERROR! In function: ", 20);
		if (func)
			write(2, func, ft_strlen(func));
		else
			write(2, "*unknown*", 9);
		write(2, ", MESSAGE: ", 11);
		if (message)
			write(2, message, ft_strlen(message));
		else
			write(2, "*nothing*", 8);
		write(2, "\n", 1);
		exit(1);
	}
}
