/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 14:11:18 by slynell           #+#    #+#             */
/*   Updated: 2020/02/19 14:13:45 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libptf.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_mem;

	if (!ptr)
		return (malloc(new_size);)
	new_mem = malloc(new_size);
	ft_assert(new_mem != NULL, __FUNCTION__, "malloc error");
	ft_memcpy(new_mem, ptr, old_size < new_size ? old_size : new_size);
	free(ptr);
	return (new_mem);
}
