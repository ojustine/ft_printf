/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libptf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slynell <slynell@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 14:04:59 by slynell           #+#    #+#             */
/*   Updated: 2020/02/19 14:05:29 by slynell          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBPTF_H
# define LIBPTF_H

# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include <wctype.h>

void		*ft_memset(void *b, int c, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t len);
int32_t		ft_moddiv(int32_t dividend, int32_t divisor, int32_t *quotient);
void		*ft_realloc(void *ptr, size_t old_size, size_t new_size);

int			ft_strnequ(char const *s1, char const *s2, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
size_t		ft_strlen(const char *s);
size_t		ft_wstrlen(const wchar_t *s);
double		ft_pow(double base, register int_fast16_t power);
long double	ft_long_pow(long double base, register int_fast16_t power);
void		ft_assert(int_fast32_t to_check, const char *func,
			const char *message);
int32_t		ft_divmod(int32_t dividend, int32_t divisor, int32_t *remainder);
size_t		ft_intlen(register int32_t n);
void		*ft_bzero(void *b, size_t n);
void		ft_memswap(void *mem1, void *mem2, size_t size);
uint32_t	ft_abs(int32_t n);
size_t		ft_longlen(register int64_t n);
int			ft_strany(char const *str, int32_t c);
void		ft_strrev(register char *begin, register char *end);
size_t		ft_uitoa_hex(uint32_t value, char *buff, int_fast16_t is_upper);
size_t		ft_ultoa_hex(uint64_t value, char *buff, int_fast16_t is_upper);
size_t		ft_uitoa_dec(uint32_t val, char *buff);
size_t		ft_uitoa_base(uint32_t value, char *buff, int_fast16_t base,
			int_fast16_t is_upper);
size_t		ft_ultoa_base(uint64_t value, char *buff, int_fast16_t base,
			int_fast16_t is_upper);

#endif
