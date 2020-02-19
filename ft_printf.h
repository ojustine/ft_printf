/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 12:18:39 by ojustine          #+#    #+#             */
/*   Updated: 2020/02/19 14:08:16 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

/*
** ========================== External headers =================================
*/

# include <stdarg.h>
# include <errno.h>
# include <float.h>
# include "ft_printf_const_data.h"
# include "ft_printf_errors.h"
# include "libptf.h"

# if defined(_POSIX_VERSION) || defined(__unix__) || defined(linux)
#  include <sys/select.h>
# endif

/*
** ====================== Consts and masks definition ==========================
*/
enum					e_printf_sizes
{
	SIZE_SHORT = (1 << 8),
	SIZE_LONG = (1 << 9),
	SIZE_LLONG = (1 << 10),
	SIZE_INTMAX = (1 << 11),
	SIZE_CHAR = (1 << 12),
	SIZE_SIZE_T = (1 << 13),
	SIZE_LDBL = (1 << 15),
	SIZE_PTR = SIZE_INTMAX
};

enum					e_printf_flags
{
	FLAG_LEFT_ALIGN = (1 << 0),
	FLAG_PLUS_SIGN = (1 << 1),
	FLAG_BLANK_SIGN = (1 << 2),
	FLAG_ALT_FORM = (1 << 3),
	FLAG_GROUP = (1 << 4),
	FLAG_TRUNCATE = (1 << 5),
	FLAG_ZERO_PAD = (1 << 6),
	FLAG_PTR = SIZE_PTR | FLAG_ALT_FORM
};

enum					e_fxd_assets
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

enum					e_ptf_assets
{
	BUFF_SIZE = 512,
	MAX_INT_BITS_NUM = ((sizeof(long long)) * 8 + 2)
};

# if defined(_POSIX_VERSION) || defined(__unix__) || defined(linux)
#  define ANSI_ECS_CODES	1
#  define ANSI_BOLD			"033[01m"
#  define ANSI_UNDERLINE	"033[04m"
#  define ANSI_REVERSED		"033[07m"
#  define ANSI_BACK_RED		"033[41m"
#  define ANSI_BACK_GRN		"033[42m"
#  define ANSI_BACK_YEL		"033[43m"
#  define ANSI_BACK_BLU		"033[44m"
#  define ANSI_FONT_RED		"033[31m"
#  define ANSI_FONT_GRN		"033[32m"
#  define ANSI_FONT_YEL		"033[33m"
#  define ANSI_FONT_BLU		"033[34m"
#  define ANSI_RESET		"033[00m"
# endif

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

typedef struct			s_ptf_info
{
	uint16_t			flags;
	int_fast32_t		width;
	int_fast32_t		prec;
	uint_fast8_t		cap;
	int32_t				fd;
	char				buff[BUFF_SIZE + 1];
	size_t				buff_index;
	size_t				printed;
	char				*output;
	va_list				ap;
	const char			*fmt;
	void				(*flush)(struct s_ptf_info*);
}						t_ptf_info;

typedef union			u_double
{
	struct
	{
		uint_fast64_t	mant		: 52;
		uint_fast64_t	exp			: 11;
		uint_fast64_t	sign		: 1;
	}					s_pts;
	double				val;
}						t_binary64;

typedef union			u_long_double
{
	struct
	{
		uint_fast64_t	mant		: 64;
		uint_fast64_t	exp			: 15;
		uint_fast64_t	sign		: 1;
	}					s_pts;
	long double			val;
}						t_binary80;

typedef struct			s_fxd
{
	uint32_t			*val;
	int_fast16_t		int_len;
	int_fast16_t		frc_len;
	int_fast16_t		f0;
}						t_fxd;

void					flush_in_string(t_ptf_info *info);
void					flush_in_file_stream(t_ptf_info *info);
t_fxd					*fxd_get_pow_2(register int_fast16_t pow,
						int_fast16_t is_long_dbl);
t_fxd					*fxd_build_mantis(uint64_t bin_mantis,
						int_fast16_t is_normal, int_fast16_t is_long_dbl);
void					fxd_dbl_mul(t_fxd *res, t_fxd *a, t_fxd *b,
						int_fast16_t is_long_dbl);
void					fxd_roundup(t_fxd *fp, const int_fast32_t prec);
void					get_floating_point_arg(t_ptf_info *info);
size_t					fxd_ftoa_dec_form(t_ptf_info *info, t_fxd *fp,
						char *buff);
size_t					fxd_ftoa_exp_form(t_ptf_info *info, t_fxd *fp,
						char *buff);
size_t					fxd_ftoa_opt_form(t_ptf_info *info, t_fxd *fp,
						char *buff);
t_fxd					*fxd_new(size_t frac_size, int_fast16_t is_long_dbl);
void					fxd_del(t_fxd *fp1, t_fxd *fp2);
void					fxd_ftoa_inf_nan(t_ptf_info *info,
						const uint64_t mantis, const char sign);
size_t					fxd_ftoa_opt_form(t_ptf_info *info, t_fxd *fp,
						char *buff);
void					fast_dtoa(t_ptf_info *info, double val);
void					fast_ldtoa(t_ptf_info *info, long double val);
size_t					ldtoa_hex_form(t_ptf_info *info, uint64_t mantis,
						int64_t exp, char *buff);

int32_t					set_prefix_fp(t_ptf_info *info, const char sign,
						const int_fast32_t val_len);
int32_t					set_prefix_num(t_ptf_info *info, const char sign,
						const int_fast16_t base, const int_fast32_t val_len);
void					padding(t_ptf_info *info, int_fast32_t pad_len,
						const char pad);
void					do_print(t_ptf_info *info, char *data, size_t size);
void					get_formatted_arg(t_ptf_info *info);
void					get_char_arg(t_ptf_info *info, int16_t is_wide_char);
void					get_string_arg(t_ptf_info *info,
						int16_t is_wide_string);
void					get_signed_arg(t_ptf_info *info, int16_t base);
void					get_unsigned_arg(t_ptf_info *info, int16_t base);
void					do_print_string(t_ptf_info *info, char *str,
						size_t size);
void					set_esc_code(t_ptf_info *info);

#endif
