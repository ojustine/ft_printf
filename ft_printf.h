/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ojustine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 12:18:39 by ojustine          #+#    #+#             */
/*   Updated: 2019/12/03 12:18:44 by ojustine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

/*
** ========================== External headers =================================
*/

# include <stdarg.h>
# include <unistd.h>
# include <errno.h>
# include <limits.h>
# include <float.h>
# include <stdlib.h>
//# include <stdint.h>
# include <sys/select.h>

/*
** ====================== Consts and masks definition ==========================
*/

# define FLAG_LEFT_ALIGN	(1U << 0U)
# define FLAG_PLUS_SIGN		(1U << 1U)
# define FLAG_BLANK_SIGN	(1U << 2U)
# define FLAG_ALT_FORM		(1U << 3U)
# define FLAG_ZERO_PAD		(1 << 4)
# define FLAG_GROUP			(1U << 5U)
# define WIDTH_IN_ARG		(1 << 6)//
# define PRINT_ARG_BY_NUM	(1 << 7)//
# define SIZE_SHORT			(1U << 8U)
# define SIZE_LONG			(1U << 9U)
# define SIZE_LLONG			(1U << 10U)
# define SIZE_INTMAX		(1U << 11U)
# define SIZE_CHAR			(1U << 12U)
# define SIZE_SIZE_T		(1U << 13U)
# define SIZE_PTR			(1U << 14U)
# define SIZE_LONG_DBL		(1U << 15U)
# define BUFF_SIZE			512

# ifdef __GNUC__
#  if !defined(__GNUC_STDC_INLINE__) && !defined(__GNUC_GNU_INLINE__)
#   define __GNUC_GNU_INLINE__ 1
#  endif
# endif

/*
** =========================== Types definition ================================
*/

typedef uint8_t				t_uint8;
typedef int8_t				t_int8;
typedef uint16_t			t_uint16;
typedef int16_t				t_int16;
typedef uint32_t			t_uint32;
typedef int32_t				t_int32;
typedef unsigned long long	t_uint64;
typedef long long			t_int64;
typedef float				t_float32;
typedef double				t_float64;
typedef long double			t_float80;

# if LLONG_MAX == 9223372036854775807
#  define IS_LONG_LONG 1
# else
#  define IS_LONG_LONG 0
# endif
# if LDBL_MAX_EXP >= 16384
#  define IS_LONG_DBL 1
# else
#  define IS_LONG_DBL 0
# endif

/*
** ======================== Structures definition ==============================
*/

typedef struct				s_printf_info
{
	t_uint16				flags;
	t_int32					width;
	t_int32					prec;
	t_uint32				length;
	char					pad;
	int						fd;
	char					buff[BUFF_SIZE + 1];
	size_t					buff_index;
	ssize_t					printed;
	t_int8					write_to_str : 1;
	char					*str_to_write;
	va_list					valist;
	const char				*format;

}							t_printf_info;

void	*ft_memset(void *b, int c, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	buffer_n_print(t_printf_info *info, void *data, size_t size);
void		print_format_arg(t_printf_info *info);
#endif