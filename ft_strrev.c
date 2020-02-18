/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 11:57:26 by slynell           #+#    #+#             */
/*   Updated: 2020/02/17 11:57:35 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libptf.h"

void	ft_strrev(register char *begin, register char *end)
{
	register char	tmp;

	while (end > begin)
	{
		tmp = *end;
		*end-- = *begin;
		*begin++ = tmp;
	}
}
