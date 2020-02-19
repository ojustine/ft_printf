/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_n_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/17 11:15:36 by slynell           #+#    #+#             */
/*   Updated: 2020/02/17 12:06:53 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	flush_in_string(t_ptf_info *info)
{
	static size_t	counter;

	ft_memcpy(info->output + counter, info->buff, info->buff_index);
	counter += info->buff_index;
	*(info->output + counter - 1) = 0;
}

void	flush_in_file_stream(t_ptf_info *info)
{
	write(info->fd, info->buff, info->buff_index);
}

void	do_print(t_ptf_info *info, char *data, size_t size)
{
	register int32_t	remaining_space;
	register size_t		data_index;

	data_index = 0;
	while ((remaining_space = BUFF_SIZE - info->buff_index) < (int)size)
	{
		if (size == 1)
			info->buff[info->buff_index] = *(char*)data;
		else
			ft_memcpy(&(info->buff[info->buff_index]), &(data[data_index]),
			remaining_space);
		size -= remaining_space;
		data_index += remaining_space;
		info->buff_index += remaining_space;
		info->printed += remaining_space;
		info->flush(info);
		info->buff_index = 0;
	}
	if (size == 1)
		info->buff[info->buff_index] = ((char*)data)[data_index];
	else
		ft_memcpy(&(info->buff[info->buff_index]), &(data[data_index]), size);
	info->buff_index += size;
	info->printed += size;
}
