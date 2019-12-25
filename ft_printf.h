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
# include <stdint.h>
# include <string.h>
# include <wctype.h>
# include "const_data.h"

# if defined(_POSIX_VERSION) || defined(__unix__) || defined(__MACH__)
#  include <sys/select.h>
# endif

/*
** ====================== Consts and masks definition ==========================
*/

# define FLAG_LEFT_ALIGN	(1U << 0U)
# define FLAG_PLUS_SIGN		(1U << 1U)
# define FLAG_BLANK_SIGN	(1U << 2U)
# define FLAG_ALT_FORM		(1U << 3U)
# define FLAG_GROUP			(1U << 4U)
# define FLAG_TRUNCATE		(1U << 5U)
# define FLAG_ZERO_PAD		(1U << 6U)
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
# define MAX_INT_BITS		(sizeof(long long) * 8 + 2)
# define FLT_MAX_LEN		FLT_MAX_EXP + 3
# define MAX(a,b)			(((a) > (b)) ? (a) : (b))

# ifdef __GNUC__
#  if !defined(__GNUC_STDC_INLINE__) && !defined(__GNUC_GNU_INLINE__)
#   define __GNUC_GNU_INLINE__ 1
#  endif
# endif

# ifndef FD_SETSIZE
#  define FD_SETSIZE 1024
# endif

/*
** =========================== Types definition ================================
*/

typedef uint16_t			t_uint16;
typedef int16_t				t_int16;
typedef uint32_t			t_uint32;
typedef int32_t				t_int32;
typedef unsigned long long	t_uint64;
typedef long long			t_int64;
typedef float				t_float;
typedef double				t_double;
typedef long double			t_ldouble;

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
	t_uint16				capitals;
	t_int32					fd;
	char					buff[BUFF_SIZE + 1];
	size_t					buff_index;
	size_t					printed;
	char					write_to_str : 1;
	char					*str_to_write;
	va_list					valist;
	const char				*fmt;

}							t_printf_info;

typedef union				u_float
{
		struct
		{
			uint32_t		mantis		: 23;
			uint32_t		bias_exp	: 8;
			uint32_t		sign		: 1;
		}					s_parts;
		float				val;
}							t_binary32;

typedef union				u_double
{
	struct
	{
		uint64_t			mantis		: 52;
		uint64_t			bias_exp	: 11;
		uint64_t			sign		: 1;
	}						s_parts;
	double					val;
}							t_binary64;

typedef union				u_long_double
{
	struct
	{
		uint64_t			mantis		: 64;
		uint64_t			bias_exp	: 15;
		uint64_t			sign		: 1;
	}						s_parts;
	long double				val;
}							t_binary80;

typedef char*				t_big_float;

void test();

void	*ft_memset(void *b, int c, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
size_t	ft_strlen(const char *s);
size_t	ft_wstrlen(wchar_t *s);
void						do_print(t_printf_info *info, char *data,
									 size_t size);
void						get_formatted_arg(t_printf_info *info);
void						print_char(t_printf_info *info,
							t_int16 is_wide_char);
void					print_string(t_printf_info *info,
							t_int16 is_wide_string);
void					print_signed_number(t_printf_info *info,
							t_int16 base);
void					print_unsigned_number(t_printf_info *info,
							t_int16 base);
#endif
