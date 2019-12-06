#include "ft_printf.h"

static inline void	buffer_flush(t_printf_info *info)
{

}

inline void			put_arg_into_buffer(t_printf_info *info, char *data,
										size_t size)
{
	if (info->width > size)
	{
		if (!(info->flags & FLAG_LEFT_ALIGN))
		{
			put_in_buffer(info, data, size);
			while ()
		}
		else
		{

		}
	}
	put_in_buffer(info, data, size);
}

void				put_in_buffer(t_printf_info *info, char *data, size_t size)
{
	t_int32		remaining_space;
	size_t		data_index;

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
