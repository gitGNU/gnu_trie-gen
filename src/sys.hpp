// Copyright (C) 2016  Stefan Vargyas
// 
// This file is part of Trie-Gen.
// 
// Trie-Gen is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Trie-Gen is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Trie-Gen.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __SYS_HPP
#define __SYS_HPP

#include <cstdio>
#include <cstdarg>

#include <stdexcept>

extern const char program[];
extern const char verdate[];
extern const char license[];

#define STRINGIFY0(S) #S
#define STRINGIFY(S)  STRINGIFY0(S)

#define PRINTF_FMT(F) \
    __attribute__ ((format(printf, F, F + 1)))
#define NORETURN \
    __attribute__ ((noreturn))
#define UNUSED \
    __attribute__ ((unused))

//!!! #define PRINTF_FMT(F)
//!!! 	[[format(printf, F, F + 1)]]
//!!! #define NORETURN
//!!! 	[[noreturn]]
//!!! #define UNUSED
//!!! 	[[unused]]

#define SYS_UNEXPECT_ERR(M, ...) \
    do { \
        Sys::unexpect_error(__FILE__, __LINE__, __func__, M, ## __VA_ARGS__); \
    } \
    while (0)

#ifdef DEBUG
# define SYS_ASSERT(E) \
    do { \
        if (!(E)) \
            Sys::assert_failed(__FILE__, __LINE__, __func__, #E); \
    } \
    while (0)
#else
# define SYS_ASSERT(E) \
    do {} while (0)
#endif

namespace Sys {

void assert_failed(
    const char* file, int line,
    const char* func, const char* expr)
    NORETURN;

void unexpect_error(
    const char* file, int line,
    const char* func, const char* msg, ...)
    PRINTF_FMT(4)
    NORETURN;

#if __cplusplus >= 201103L
# define STATIC_ASSERT2(E, M) \
    static_assert(E, M)
#else
template<bool b>
struct static_assert_t;

template<>
struct static_assert_t<true>
{ enum { value }; };

# define STATIC_ASSERT2(E, M) \
    do { \
        (void) Sys::static_assert_t<(X)>::value; \
    } \
    while (0)
#endif

#define STATIC_ASSERT(E) \
    STATIC_ASSERT2(E, #E)

struct Error : public std::runtime_error
{
    explicit Error(const char* what) :
        std::runtime_error{what}
    {}
};

template<typename T>
inline void verror(const char* msg, va_list args)
{
    char buf[256];
    vsnprintf(buf, sizeof buf -1, msg, args);
    buf[255] = 0;

    throw T(buf);
}

template<typename T>
inline void error(const char* msg, ...)
    PRINTF_FMT(1);

template<typename T>
inline void error(const char* msg, ...)
{
    STATIC_ASSERT(
        CONFIG_VA_END_NOOP);
    va_list args;
    va_start(args, msg);
    verror<T>(msg, args);
    //!!!VA_END va_end(args);
}

} // namespace Sys

#endif /* __SYS_HPP */


