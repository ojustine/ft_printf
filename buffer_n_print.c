#include "ft_printf.h"

static inline void	buffer_flush(t_printf_info *info)
{

}

static inline void	padding(t_printf_info *info, size_t size)
{
	const char	*zero_pad = "0000";
	const char	*blank_pad = "    ";
	char		*curr_pad;

	curr_pad = (char*)((info->flags & FLAG_ZERO_PAD) ? zero_pad : blank_pad);
	while (size >= 4)
	{
		put_in_buffer(info, curr_pad, 4);
		size -= 4;
	}
	while (size--)
		put_in_buffer(info, curr_pad, 1);
}

void				put_arg_in_buffer(t_printf_info *info, char *data,
										size_t size)
{
	t_int32		pad_num;
	t_uint32	to_print;

	if (info->flags & FLAG_TRUNCATE)
		to_print = (info->prec < size) ? info->prec : size;
	else
		to_print = size;
	if ((pad_num = info->width - to_print) > 0)
	{
		if (info->flags & FLAG_LEFT_ALIGN)
		{
			put_in_buffer(info, data, to_print);
			padding(info, pad_num);
		}
		else
		{
			padding(info, pad_num);
			put_in_buffer(info, data, to_print);
		}
	}
	else
		put_in_buffer(info, data, to_print);
}

void				put_in_buffer(t_printf_info *info, char *data, size_t size)
{
	register t_int32		remaining_space;
	register size_t			data_index;

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
		write(info->fd, info->buff, info->buff_index);
		info->buff_index = 0;
	}
	if (size == 1)
		info->buff[info->buff_index] = *(char*)data;
	else
		ft_memcpy(&(info->buff[info->buff_index]), &(data[data_index]), size);
	info->buff_index += size;
	info->printed += size;
}
