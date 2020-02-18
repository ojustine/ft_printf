cmake_minimum_required(VERSION 3.14)
project(ft_printf C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ft_printf
        ft_memset.c
        ft_printf.c
        ft_printf.h
        main.c
        ft_memcpy.c
        buffer_n_print.c
        get_format_arg.c
        string_processing.c
        ft_strlen.c
        ft_wstrlen.c
        number_processing.c
        float_processing.c
        ft_ceil.c ft_pow.c
        ft_long_pow.c
        float_processing_fxd_dbl.c
        ft_memmove.c
        ft_assert.c
        ft_moddiv.c
        ft_divmod.c
        float_processing_ef.c
        ft_intlen.c
        ft_longlen.c
        ft_bzero.c
        ft_memswap.c
        ft_printf_utils.c
        ft_abs.c
        float_processing_ag.c ft_strany.c ft_strrev.c
        float_processing_fast_dtoa.c ft_uitoa_hex.c ft_ultoa_hex.c ft_uitoa_dec.c ft_uitoa_base.c ft_ultoa_base.c ft_realloc.c libptf.h)

#target_compile_options (ft_printf PRIVATE -Werror)