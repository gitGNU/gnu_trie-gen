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

#ifndef __CONFIG_H

#ifndef __GNUC__
#error we need a GCC compiler
#endif

#ifndef __GNUC_MINOR__
#error __GNUC_MINOR__ is not defined
#endif

#ifndef __GNUC_PATCHLEVEL__
#error __GNUC_PATCHLEVEL__ is not defined
#endif

#define GCC_VERSION \
    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

//
// CONFIG_MEM_ALIGNOF
//

// stev: CONFIG_MEM_ALIGNOF establishes whether the compiler is
// providing an '__alignof__' builtin function which allows to
// inquire about the minimum alignment required by a type or a
// variable. GCC does have such a builtin: I made only a couple
// of investigations for versions upward from v4.0.0 (look into
// http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Alignment.html).

// Note that as of GCC v4.8.0 the builtin function is no longer
// needed, since ISO/IEC 14882:2011 specifies the existence of
// the 'alignof()' unary operator which applied to a 'type-id'
// gets back the alignment requirement of the type in question.
#if GCC_VERSION < 40000 || GCC_VERSION >= 40800
#define CONFIG_MEM_ALIGNOF 0
#else
#define CONFIG_MEM_ALIGNOF 1
#endif

//
// CONFIG_TWOS_COMPLEMENT_MACHINE
// CONFIG_ARITHMETIC_SHIFT_MACHINE
// CONFIG_BITOPS_SIGN_AND_BITS_MACHINE
//

// stev: CONFIG_TWOS_COMPLEMENT_MACHINE establishes whether the
// tandem of the machine/the compiler represents signed integer
// types by two's complement or not. (C99 6.2.6.2)

// stev: CONFIG_ARITHMETIC_SHIFT_MACHINE establishes whether the
// tandem of the machine/the compiler implements right shifts of
// signed types as 'arithmetic shifts' or it does not. (C99 6.5)

// stev: CONFIG_BITOPS_SIGN_AND_BITS_MACHINE establishes whether
// the tandem of the machine/the compiler implements the bitwise
// operations the way GCC does (at least from v4.0.0 upward: see
// below).

// stev: from at least version 4.0.0 upward, GCC represents signed
// integer types by *two's complement* and implements right shifts
// of negative integer types as *arithmetic shifts*. The sign of
// the shifted value is preserved. (Note that I did not checked
// the documentation of versions prior to that particular 4.0.0,
// http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/Integers-implementation.html:
//
//   The results of some bitwise operations on signed integers
//   (C90 6.3, C99 6.5):
//
//   Bitwise operators act on the representation of the value
//   including both the sign and value bits, where the sign bit
//   is considered immediately above thehighest-value value bit.
//   Signed `>>' acts on negative numbers by sign extension.
//
//   GCC does not use the latitude given in C99 only to treat
//   certain aspects of signed `<<' as undefined, but this is
//   subject to change.
//
#if GCC_VERSION < 40000
#define CONFIG_TWOS_COMPLEMENT_MACHINE 0
#define CONFIG_ARITHMETIC_SHIFT_MACHINE 0
#define CONFIG_BITOPS_SIGN_AND_BITS_MACHINE 0
#else
#define CONFIG_TWOS_COMPLEMENT_MACHINE 1
#define CONFIG_ARITHMETIC_SHIFT_MACHINE 1
#define CONFIG_BITOPS_SIGN_AND_BITS_MACHINE 1
#endif

//
// CONFIG_PTR_TO_INT_IDENTOP
//

// stev: CONFIG_PTR_TO_INT_IDENTOP establishes whether the compiler
// implements the conversion from pointer types to integer types of
// equal size as an identity operation. That is that the bits which
// result upon casting a pointer to an integer of the same size are
// identical with those of the pointer representation itself.

// stev: the implementation of GCC at least upward from v4.0.0 does
// precisely this (see http://gcc.gnu.org/onlinedocs/gcc-4.0.0/gcc/
// Arrays-and-pointers-implementation.html):
//
//   The result of converting a pointer to an integer or vice versa
//   (C90 6.3.4, C99 6.3.2.3):
//
//   A cast from pointer to integer discards most-significant bits
//   if the pointer representation is larger than the integer type,
//   sign-extends [1] if the pointer representation is smaller than
//   the integer type, otherwise the bits are unchanged.
//
//   Footnotes
//
//   [1] Future versions of GCC may zero-extend, or use a target-
//       defined ptr_extend pattern. Do not rely on sign extension.
//
// Note that I only looked into the GCC documentation of versions
// between v4.0.0 and v4.9.0.
#if GCC_VERSION < 40000 || GCC_VERSION > 40900
#define CONFIG_PTR_TO_INT_IDENTOP 0
#else
#define CONFIG_PTR_TO_INT_IDENTOP 1
#endif

//
// CONFIG_VA_END_NOOP
//

// stev: CONFIG_VA_END_NOOP establishes whether the compiler does
// not require that invocations of 'va_start' macro be matched by
// a corresponding 'va_end' call -- in other words, the parameter
// shall evaluate to 'true' iff 'va_end' is a no-operation macro
// (or function). Note that the C standard (C99 7.15.1) says that
// each call to 'va_start' shall be paired by a call to 'va_end'.

// stev: the implementation of GCC v4.x (0 <= x <= 9) boils down
// the 'va_end' macro to a built-in function '__builtin_va_end'.
// This function is indeed a trivial no-operation (look into the
// source code gcc/builtins.{def,c}). Other versions of GCC might
// well implement the same behaviour -- but, since this issue is
// a tricky one, CONFIG_VA_END_NOOP should only be set to 'true'
// following a positive assurance about the things implied.
#if GCC_VERSION < 40000 || GCC_VERSION > 40900
#define CONFIG_VA_END_NOOP 0
#else
#define CONFIG_VA_END_NOOP 1
#endif

#endif /* __CONFIG_H */

