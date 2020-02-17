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
# include "ft_printf_const_data.h"

# if defined(_POSIX_VERSION) || defined(__unix__) || defined(linux)
#  include <sys/select.h>
# endif

/*
** ====================== Consts and masks definition ==========================
*/
enum						e_printf_flags
{
	FLAG_LEFT_ALIGN = (1 << 0),
	FLAG_PLUS_SIGN = (1 << 1),
	FLAG_BLANK_SIGN = (1 << 2),
	FLAG_ALT_FORM = (1 << 3),
	FLAG_GROUP = (1 << 4),
	FLAG_TRUNCATE = (1 << 5),
	FLAG_ZERO_PAD = (1 << 6),
	PRINT_ARG_BY_NUM = (sizeof(long long))
};

enum						e_printf_sizes
{
	SIZE_SHORT = (1 << 8),
	SIZE_LONG = (1 << 9),
	SIZE_LLONG = (1 << 10),
	SIZE_INTMAX = (1 << 11),
	SIZE_CHAR = (1 << 12),
	SIZE_SIZE_T = (1 << 13),
	SIZE_PTR = (1 << 14),
	SIZE_LDBL = (1 << 15)
};

enum						e_fxd_assets
{
	FP_R_LIMITER = 1000000000,
	FP_R_TOP = FP_R_LIMITER / 10,
	FP_R_LEN = 9,
	FP_R_SIZE = sizeof(uint32_t),

	FP_D_POINT = (DBL_MAX_10_EXP / FP_R_LEN) + 2,
	FP_D_MAX_PREC = 1080,
	FP_D_LEN = (FP_D_POINT + FP_D_MAX_PREC / FP_R_LEN),
	FP_D_CHAR_LEN = DBL_MAX_10_EXP + FP_D_MAX_PREC,

	FP_LD_MAX_PREC = 16445,
	FP_LD_POINT = (LDBL_MAX_10_EXP / FP_R_LEN) + 2,
	FP_LD_LEN = (FP_LD_POINT + FP_LD_MAX_PREC / FP_R_LEN) + 3,
	FP_LD_CHAR_LEN = DBL_MAX_10_EXP + FP_LD_MAX_PREC,
	FP_LD_64BIT = 1ULL << 63
};

# define BUFF_SIZE			512
# define MAX_INT_BITS_NUM	((sizeof(long long)) * 8 + 2)

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

# if LLONG_MAX == 9223372036854775807
#  define IS_LONG_LONG 1
# else
#  define IS_LONG_LONG 0
# endif
# if LDBL_MAX_EXP == 16384
#  define IS_LONG_DBL_DEFINED 1
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
	int_fast32_t			prec;
	uint_fast8_t			cap;
	int32_t					fd;
	char					buff[BUFF_SIZE + 1];
	size_t					buff_index;
	size_t					printed;
	char					*output;
	va_list					ap;
	const char				*fmt;
	int						(*flush)(int, const void*, unsigned int);
}							t_printf_info;

typedef union				u_double
{
	struct
	{
		uint_fast64_t		mant		: 52;
		uint_fast64_t		exp			: 11;
		uint_fast64_t		sign		: 1;
	}						s_pts;
	double					val;
}							t_binary64;

typedef union				u_long_double
{
	struct
	{
		uint_fast64_t		mant		: 64;
		uint_fast64_t		exp			: 15;
		uint_fast64_t		sign		: 1;
	}						s_pts;
	long double				val;
}							t_binary80;

typedef struct				s_fxd
{
	uint32_t				*val;
	int_fast16_t			int_len;
	int_fast16_t			frc_len;
	int_fast16_t			f0;
}							t_fxd;

void	*ft_memset(void *b, int c, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
size_t	ft_strlen(const char *s);
size_t	ft_wstrlen(wchar_t *s);
double	ft_pow(double base, int_fast16_t power);
long double	ft_long_pow(long double base, int_fast16_t power);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	ft_assert(int_fast32_t to_check, const char *func, const char *message);
int32_t	ft_moddiv(int32_t dividend, int32_t divisor, int32_t *quotient);
int32_t	ft_divmod(int32_t dividend, int32_t divisor, int32_t *remainder);
size_t	ft_intlen(int32_t n);
void	*ft_bzero(void *b, size_t n);
void				ft_memswap(void *mem1, void *mem2, size_t size);
uint32_t	ft_abs(int32_t n);
size_t	ft_longlen(int64_t n);
int	ft_strany(char const *str, int32_t c);
void	ft_strrev(char *begin, char *end);
size_t	ft_uitoa_hex(uint32_t value, char* buff, int_fast16_t is_upper);
size_t	ft_ultoa_hex(uint64_t value, char* buff, int_fast16_t is_upper);

t_fxd				*fxd_get_pow_2(int_fast16_t pow, int_fast16_t is_long_dbl);
t_fxd				*fxd_build_mantis(uint64_t bin_mantis,
										int_fast16_t is_normal, int_fast16_t is_long_dbl);
void				fxd_dbl_mul(t_fxd *res, t_fxd *a, t_fxd *b,
								int_fast16_t is_long_dbl);
void				fxd_roundup(t_fxd *fp, int_fast32_t prec);
void	get_floating_point_arg(t_printf_info *info);
size_t					fxd_ftoa_dec_form(t_printf_info *info, t_fxd *fp, char *buff);
size_t					fxd_ftoa_exp_form(t_printf_info *info, t_fxd *fp, char *buff);
size_t					fxd_ftoa_opt_form(t_printf_info *info, t_fxd *fp, char *buff);
t_fxd	*fxd_new(size_t frac_size, int_fast16_t is_long_dbl);
void	fxd_del(t_fxd *fp1, t_fxd *fp2);
void					fxd_ftoa_inf_nan(t_printf_info *info, uint64_t mantis, char sign);
int32_t				set_prefix_fp(t_printf_info *info, const char sign, const int_fast32_t val_len);
size_t					fxd_ftoa_opt_form(t_printf_info *info, t_fxd *fp, char *buff);
void					fast_dtoa(t_printf_info *info, double val);

int32_t	set_prefix_num(t_printf_info *info, const char sign,
						  const int_fast16_t base, const int_fast32_t val_len);
void				padding(t_printf_info *info, int_fast32_t pad_len, const char pad);

void						do_print(t_printf_info *info, char *data,
									 size_t size);
void						get_formatted_arg(t_printf_info *info);
void						get_char_arg(t_printf_info *info,
										 int16_t is_wide_char);
void					get_string_arg(t_printf_info *info,
									   int16_t is_wide_string);
void					get_signed_arg(t_printf_info *info,
									   int16_t base);
void					get_unsigned_arg(t_printf_info *info,
										 int16_t base);
void					do_print_string(t_printf_info *info, char *str,
										size_t size);

#endif
