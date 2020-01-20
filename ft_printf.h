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
enum						e_flags
{
	FLAG_LEFT_ALIGN = (1 << 0),
	FLAG_PLUS_SIGN = (1 << 1),
	FLAG_BLANK_SIGN = (1 << 2),
	FLAG_ALT_FORM = (1 << 3),
	FLAG_GROUP = (1 << 4),
	FLAG_TRUNCATE = (1 << 5),
	FLAG_ZERO_PAD = (1 << 6),
	PRINT_ARG_BY_NUM = (1 << 7)
};

enum						e_sizes
{
	SIZE_SHORT = (1 << 8),
	SIZE_LONG = (1 << 9),
	SIZE_LLONG = (1 << 10),
	SIZE_INTMAX = (1 << 11),
	SIZE_CHAR = (1 << 12),
	SIZE_SIZE_T = (1 << 13),
	SIZE_PTR = (1 << 14),
	SIZE_LONG_DBL = (1 << 15)
};

# define BUFF_SIZE			512
# define MAX_INT_BITS_NUM	(sizeof(long long) * 8 + 2)
# define FLT_MAX_LEN		(FLT_MAX_10_EXP + 3)
# define DBL_MAX_LEN		(DBL_MAX_10_EXP + 3)
# define LDBL_MAX_LEN		(LDBL_MAX_10_EXP + 3)
# define MAX(a,b)			(((a) > (b)) ? (a) : (b))
# define MIN(a,b)			(((a) < (b)) ? (a) : (b))

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

typedef uint8_t				t_u8;
typedef int8_t				t_s8;
typedef uint16_t			t_u16;
typedef int16_t				t_s16;
typedef uint32_t			t_u32;
typedef int32_t				t_s32;
typedef uint64_t 			t_u64;
typedef int64_t			t_s64;
typedef double				t_double;
typedef long double			t_ldouble;

# if LLONG_MAX == 9223372036854775807
#  define IS_LONG_LONG 1
# else
#  define IS_LONG_LONG 0
# endif
# if LDBL_MAX_EXP == 16384
#  define IS_LONG_DBL 1
# else
#  define IS_LONG_DBL 0
# endif

/*
** ======================== Structures definition ==============================
*/

typedef struct				s_printf_info
{
	uint16_t				flags;
	int_fast32_t			width;
	int_fast16_t			prec;
	uint_fast8_t			capitals;
	int32_t					fd;
	char					buff[BUFF_SIZE + 1];
	size_t					buff_index;
	size_t					printed;
	char					write_to_str : 1;
	char					*str_to_write;
	va_list					ap;
	const char				*fmt;
}							t_printf_info;

typedef union				u_float
{
	struct
	{
		uint_fast32_t		mantis		: 23;
		uint_fast32_t		bias_exp	: 8;
		uint_fast32_t		sign		: 1;
	}						s_parts;
	float					val;
}							t_binary32;

typedef union				u_double
{
	struct
	{
		uint_fast64_t		mantis		: 52;
		uint_fast64_t		bias_exp	: 11;
		uint_fast64_t		sign		: 1;
	}						s_parts;
	double					val;
}							t_binary64;

typedef union				u_long_double
{
	struct
	{
		uint_fast64_t		mantis		: 64;
		uint_fast64_t		bias_exp	: 15;
		uint_fast64_t		sign		: 1;
	}						s_parts;
	long double				val;
}							t_binary80;

typedef struct				s_big_float
{
	char					digits[FLT_MAX_LEN];
	int_fast16_t			point_pos;
	int_fast16_t			length;
	int_fast8_t				sign;
}							t_big_float;

typedef struct				s_fxd_point_dbl
{
	uint32_t				ints[DBL_MAX_10_EXP + 1];
	uint32_t				frac[DBL_MAX_10_EXP + 1];
	int_fast16_t			int_inx;
	int_fast16_t			frac_inx;
}							t_fxd_point_dbl;

void test();

void	*ft_memset(void *b, int c, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
size_t	ft_strlen(const char *s);
size_t	ft_wstrlen(wchar_t *s);
double	ft_pow(double base, int_fast16_t power);
long double	ft_long_pow(long double base, int_fast16_t power);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	ft_assert(int_fast32_t to_check, const char *func, const char *message);

void	bf_parse(t_big_float *f, const char *str);
void	bf_shift_right(t_big_float *a, const int_fast16_t shift);
void	bf_shift_left(t_big_float *a, const int_fast16_t shift);
void	bf_move_value(t_big_float *a, const int_fast16_t prec,
					  int_fast8_t is_move_to_tail);
void	bf_add(t_big_float *a, t_big_float *b, t_big_float *res,
			   const int_fast16_t prec);
void	bf_multiply(t_big_float *a, t_big_float *b, t_big_float *res,
					int_fast16_t prec);

void				fxd_point_build_mantis(t_binary64 bin64,
										   t_fxd_point_dbl *fxd_point_dbl);

void						do_print(t_printf_info *info, char *data,
									 size_t size);
void						get_formatted_arg(t_printf_info *info);
void						print_char(t_printf_info *info,
									   t_s16 is_wide_char);
void					print_string(t_printf_info *info,
									 t_s16 is_wide_string);
void					print_signed_number(t_printf_info *info,
											t_s16 base);
void					print_unsigned_number(t_printf_info *info,
											  t_s16 base);
#endif
