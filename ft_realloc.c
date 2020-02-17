#include "ft_printf.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_mem;

	if (!ptr)
		return malloc(new_size);
	new_mem = malloc(new_size);
	ft_assert(new_mem != NULL, __FUNCTION__, "malloc error");
	ft_memcpy(new_mem, ptr, old_size < new_size ? old_size : new_size);
	free(ptr);
	return (new_mem);
}
