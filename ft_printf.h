#ifndef FT_PRINTF
#define FT_PRINTF

/*
** ========================== External headers =================================
*/

# include <stdarg.h>
# include <errno.h>
# include <limits.h>
# include <float.h>
# include <stdlib.h>

/*
** ====================== Consts and masks definition ==========================
*/

# define FLAG_LEFT_ALIGN	(1 << 0)
# define FLAG_SHOW_SIGN		(1 << 1)
# define FLAG_BLANK_SIGN	(1 << 2)
# define FLAG_ALT_FORM		(1 << 3)
# define FLAG_ZERO_PAD		(1 << 4)
# define FLAG_GROUP			(1 << 5)
# define WIDTH_IN_ARG		(1 << 6)
# define PREC_IN_ARG		(1 << 7)
# define SIZE_INT16			(1 << 8)
# define SIZE_INT32			(1 << 9)
# define SIZE_INT64			(1 << 10)
# define SIZE_INTMAX		(1 << 11)
# define SIZE_CHAR			(1 << 12)
# define SIZE_SIZE_T		(1 << 13)
# define SIZE_PTR			(1 << 14)
# define SIZE_LONG_DBL		(1 << 15)
# define BUFF_SIZE			64

/*
** =========================== Types definition ================================
*/

typedef unsigned short		t_uint16;
typedef short				t_int16;
typedef unsigned long 		t_uint32;
typedef long				t_int32;
typedef float				t_float32;
typedef double				t_float64;

# if LONG_LONG_MAX > LONG_MAX
typedef long long			t_int64;
typedef unsigned long long	t_uint64;
#  define IS_LONG_LONG 1
# else
typedef t_int32 			t_int64;
typedef t_uint32			t_uint64;
#  define IS_LONG_LONG 0
# endif
# if LDBL_MAX_EXP > DBL_MAX_EXP
typedef long double			t_float128;
#  define IS_LONG_DBL 1
# else
typedef t_float64 			t_float128;
#  define IS_LONG_DBL 0
# endif

/*
** ======================== Structures definition ==============================
*/

typedef struct				s_format_spec
{
	t_uint16				flags;
	t_int32					width;
	t_int32					prec;
	char 					pad;
	int 					fd;
	char 					buff[BUFF_SIZE];
	size_t					buff_index;
	va_list					valist;
	char 					*format;


}							t_format_spec;

void	*ft_memset(void *b, int c, size_t n);
#endif
