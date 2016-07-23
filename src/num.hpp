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

#ifndef __NUM_HPP
#define __NUM_HPP

#include <limits>
#include <type_traits>

#include "sys.hpp"

namespace Num {

template<typename T>
struct num_name_t
{
    static const char name[];
    static const size_t size;
};

template<typename T>
constexpr const char* num_name()
{ return num_name_t<T>::name; }

template<typename T>
constexpr size_t num_name_len()
{ return num_name_t<T>::size - 1; }

#define DEFINE_NUM_NAME(T) \
template<> const char num_name_t<T>::name[] = \
    #T; \
template<> const size_t num_name_t<T>::size = \
    Ext::array_size(num_name_t<T>::name);

template<typename T>
constexpr bool is_integer()
{ return std::numeric_limits<T>::is_integer; }

template<typename T>
constexpr bool is_signed()
{ return std::numeric_limits<T>::is_signed; }

template<typename T>
constexpr bool is_unsigned()
{ return !is_signed<T>(); }

template<typename T>
constexpr bool is_signed_int()
{ return is_integer<T>() && is_signed<T>(); }

template<typename T>
constexpr bool is_unsigned_int()
{ return is_integer<T>() && !is_signed<T>(); }

template<typename T>
constexpr bool is_enum()
{ return std::is_enum<T>::value; }

template<typename t>
constexpr bool is_integer_or_enum()
{ return is_integer<t>() || is_enum<t>(); }

template<typename T>
constexpr bool is_integral()
{ return std::is_integral<T>::value; }

template<typename T>
constexpr bool is_floating_point()
{ return std::is_floating_point<T>::value; }

template<typename T>
constexpr bool is_arithmetic()
{ return std::is_arithmetic<T>::value; }

template<typename T>
constexpr bool is_pointer()
{ return std::is_pointer<T>::value; }

template<typename T>
constexpr bool is_array()
{ return std::is_array<T>::value; }

template<typename T>
constexpr bool is_const()
{ return std::is_const<T>::value; }

template<typename T, typename U>
constexpr bool is_same()
{ return std::is_same<T, U>::value; }

template<typename T, typename U>
constexpr bool is_cv_same()
{
    return is_same<
        typename std::remove_cv<T>::type,
        typename std::remove_cv<U>::type>();
}

// stev: according to ISO/IEC 14882:2003(E)
// and ISO/IEC 14882:2011 3.9.1.1, plain
// char, signed and unsigned char are three
// distinct types. Identical statements are
// made by ISO/IEC 9899:1999(TC3) and ISO/IEC
// 9899:2011 6.2.5.15 and 6.2.5 and J.3.4;
// ISO/IEC 9899:1999(TC3) footnote 35;
// ISO/IEC 9899:2011 footnote 45.

template<typename T>
constexpr bool is_char_pointer()
{
    typedef
        typename std::remove_pointer<T>::type
        base_t;
    return
        is_pointer<T>()
        && (is_cv_same<base_t, char>()
            || is_cv_same<base_t, unsigned char>()
            || is_cv_same<base_t, signed char>());
}

template<typename T>
constexpr T min()
{ return std::numeric_limits<T>::min(); }

template<typename T>
constexpr T max()
{ return std::numeric_limits<T>::max(); }

template<typename T>
constexpr size_t digits()
{ return std::numeric_limits<T>::digits; }

template<typename T>
constexpr size_t digits10()
{ return std::numeric_limits<T>::digits10; }

template<typename T>
constexpr typename std::enable_if<
    is_integer<T>(),
bool>::type
    sign()
{ return is_signed<T>(); }

template<typename T>
constexpr typename std::enable_if<
    is_integer<T>(),
size_t>::type
    width()
{ return digits<T>(); }

template<typename R, typename V>
inline typename std::enable_if<
    (width<R>() == width<V>()),
bool>::type
    check_limits(V v UNUSED)
{
    return true;
}

template<typename R, typename V>
inline typename std::enable_if<
    (sign<R>() == sign<V>()) &&
    (width<R>() > width<V>()),
bool>::type
    check_limits(V v UNUSED)
{
    return true;
}

template<typename R, typename V>
inline typename std::enable_if<
    (sign<R>() == false) &&
    (sign<V>() == false) &&
    (width<R>() < width<V>()),
bool>::type
    check_limits(V v)
{
    return
        v <= max<R>();
}

template<typename R, typename V>
inline typename std::enable_if<
    (sign<R>() == true) &&
    (sign<V>() == true) &&
    (width<R>() < width<V>()),
bool>::type
    check_limits(V v)
{
    return
        v >= min<R>() &&
        v <= max<R>();
}

template<typename R, typename V>
inline typename std::enable_if<
    (sign<R>() == false) &&
    (sign<V>() == true) &&
    (width<R>() > width<V>()),
bool>::type
    check_limits(V v)
{
    STATIC_ASSERT(
        //!!!CAST
        min<R>() == 0 &&
        V(min<R>()) > min<V>());
    return
        //!!!CAST
        v >= V(min<R>());
}

template<typename R, typename V>
inline typename std::enable_if<
    (sign<R>() == true) &&
    (sign<V>() == false) &&
    (width<R>() > width<V>()),
bool>::type
    check_limits(V v UNUSED)
{
    return true;
}

template<typename R, typename V>
inline typename std::enable_if<
    (sign<R>() == false) &&
    (sign<V>() == true) &&
    (width<R>() < width<V>()),
bool>::type
    check_limits(V v)
{
    STATIC_ASSERT(
        //!!!CAST
        min<R>() == 0 &&
        max<R>() > 0 &&
        V(min<R>()) > min<V>() &&
        V(max<R>()) < max<V>());
    return
        //!!!CAST
        v >= V(min<R>()) &&
        v <= V(max<R>());
}

template<typename R, typename V>
inline typename std::enable_if<
    (sign<R>() == true) &&
    (sign<V>() == false) &&
    (width<R>() < width<V>()),
bool>::type
    check_limits(V v)
{
    STATIC_ASSERT(
        //!!!CAST
        max<R>() > 0 &&
        V(max<R>()) < max<V>());
    return
        //!!!CAST
        v <= V(max<R>());
}

template<typename R, typename V>
inline typename std::enable_if<
    is_integer<V>() &&
    is_integer<R>(),
R>::type
    integer_cast(V v)
{
    SYS_ASSERT(
        check_limits<R>(v));
    return
        static_cast<R>(v);
}

template<typename R, typename V>
inline typename std::enable_if<
    is_integer<V>() &&
    is_integer<R>(),
bool>::type
    integer_cast(V v, R& r)
{
    if (check_limits<R>(v)) {
        r = static_cast<R>(v);
        return true;
    }
    return false;
}

template<typename R, typename V>
inline typename std::enable_if<
    !is_cv_same<R, V>() &&
    is_enum<V>() &&
    is_integer<R>(),
R>::type
    integer_cast(V v)
{
    typedef
        typename
            std::underlying_type<V>::type
        enum_t;
    STATIC_ASSERT(
        is_integer<enum_t>());
    return
        integer_cast<R>(
            static_cast<enum_t>(v));
}

template<typename R, typename V>
inline typename std::enable_if<
    !is_cv_same<R, V>() &&
    is_enum<V>() &&
    is_integer<R>(),
bool>::type
    integer_cast(V v, R& r)
{
    typedef
        typename
            std::underlying_type<V>::type
        enum_t;
    STATIC_ASSERT(
        is_integer<enum_t>());
    return
        integer_cast<R>(
            static_cast<enum_t>(v),
            r);
}

template<typename R, typename V>
inline typename std::enable_if<
    is_integer<V>() &&
    is_enum<R>(),
R>::type
    enum_cast(V v)
{
    typedef
        typename
            std::underlying_type<R>::type
        enum_t;
    STATIC_ASSERT(
        is_integer<enum_t>());
    return
        static_cast<R>(
            integer_cast<enum_t>(v));
}

template<typename R, typename V>
inline typename std::enable_if<
    is_integer<V>() &&
    is_enum<R>(),
bool>::type
    enum_cast(V v, R& r)
{
    typedef
        typename
            std::underlying_type<R>::type
        enum_t;
    STATIC_ASSERT(
        is_integer<enum_t>());
    enum_t e;
    if (integer_cast<enum_t>(v, e)) {
        r = static_cast<R>(e);
        return true;
    }
    return false;
}

template<typename T>
class box_t final
{
public:
    typedef
        typename
            std::enable_if<
                is_integer<T>(),
                T>::type
            type_t;

    box_t(box_t&&) = default;
    box_t(const box_t&) = default;

    box_t& operator=(box_t&&) = default;
    box_t& operator=(const box_t&) = default;

    // stev: the constructors below
    // is not 'explicit' on purpose

    //!!!ENABLE_IF
    template<typename V>
    box_t(box_t<V> a) :
        v{integer_cast<type_t>(a.v)}
    {}

    //!!!ENABLE_IF
    //template<typename V>
    //box_t(
    //	typename std::enable_if<
    //		!is_cv_same<T, V>(),
    //		box_t<V>>::type a) :
    //	v{integer_cast<type_t>(a.v)}
    //{}

    //!!!ENABLE_IF
    template<typename V>
    box_t(V a) :
        v{integer_cast<type_t>(a)}
    {
        STATIC_ASSERT(
            is_integer_or_enum<V>());
    }

    //!!!ENABLE_IF
    //template<typename V>
    //box_t(
    //	typename std::enable_if<
    //		is_integer_or_enum<V>(),
    //		V>::type a) :
    //	v{integer_cast<type_t>(a)}
    //{}

    template<typename V>
    typename std::enable_if<
        is_integer_or_enum<V>(),
        box_t&>::type
    operator=(V a)
    {
        v = integer_cast<type_t>(a);
        return *this;
    }

    template<typename V>
    typename std::enable_if<
        is_integer<V>(),
        box_t&>::type
    operator=(box_t<V> a)
    { return operator=(a.v); }

    //!!!ENABLE_IF
    //template<typename V>
    //operator typename std::enable_if<
    //	is_integer_or_enum<V>(),
    //	V>::type () const
    //{ return integer_cast<V>(v); }

    //!!!ENABLE_IF
    template<typename V>
    operator V() const
    { return integer_cast<V>(v); }

    template<typename V>
    typename std::enable_if<
        is_integer_or_enum<V>(),
        V>::type
    as() const
    { return integer_cast<V>(v); }

    template<typename V>
    typename std::enable_if<
        is_integer<V>(),
        box_t<V>>::type
    as_box() const
    { return box_t<V>{v}; }

    type_t as() const
    { return v; }

    type_t operator()() const
    { return v; }

    template<typename V>
    typename std::enable_if<
        is_integer<V>(),
        box_t>::type
    operator-(V a) const
    { return box_t{v} -= a; }

    template<typename V>
    typename std::enable_if<
        is_integer<V>(),
        box_t>::type
    operator-(box_t<V> a)
    { return operator-(a.v); }

    template<typename V>
    typename std::enable_if<
        is_integer<V>(),
        box_t>::type
    operator+(V a) const
    { return box_t{v} += a; }

    template<typename V>
    typename std::enable_if<
        is_integer<V>(),
        box_t>::type
    operator+(box_t<V> a)
    { return operator+(a.v); }

    template<typename V>
    typename std::enable_if<
        is_integer<V>(),
        box_t&>::type
    operator-=(V a)
    {
        v -= integer_cast<type_t>(a);
        return *this;
    }

    template<typename V>
    typename std::enable_if<
        is_integer<V>(),
        box_t&>::type
    operator-=(box_t<V> a)
    { return operator-=(a.v); }

    template<typename V>
    typename std::enable_if<
        is_integer<V>(),
        box_t&>::type
    operator+=(V a)
    {
        v += integer_cast<type_t>(a);
        return *this;
    }

    template<typename V>
    typename std::enable_if<
        is_integer<V>(),
        box_t&>::type
    operator+=(box_t<V> a)
    { return operator+=(a.v); }

    box_t& operator--()
    { v --; return *this; }

    box_t& operator++()
    { v --; return *this; }

    box_t operator--(int)
    { box_t r{v}; v --; return r; }

    box_t operator++(int)
    { box_t r{v}; v ++; return r; }

private:
    type_t v;
};

template<unsigned N, typename T>
inline size_t digits(T v)
{
    STATIC_ASSERT(
        is_unsigned_int<T>() &&
        N > 0);
    size_t r = 0;
    do {
        v /= N;
        r ++;
    }
    while (v > 0);
    return r;
}

typedef long long widest_sint_t;
typedef unsigned long long widest_uint_t;

template<typename T>
constexpr bool is_widest_int()
{
    return
        is_cv_same<T, widest_sint_t>() ||
        is_cv_same<T, widest_uint_t>();
}

template<typename T>
struct widest_int_t
{
    typedef
        typename std::enable_if<
            is_integer<T>(),
            typename
                std::conditional<
                    is_unsigned<T>(),
                    widest_uint_t,
                    widest_sint_t
                >::type
            >::type
        type;
};

template<size_t N, typename T>
constexpr typename std::enable_if<
    is_unsigned_int<T>(),
T>::type
    max()
{
    STATIC_ASSERT(
        N < digits<T>());
    return (T(1) << N) - 1;
}

template<size_t N, typename T>
inline typename std::enable_if<
    is_unsigned_int<T>(),
bool>::type
    check_bits_cast(T v)
{ return v <= max<N, T>(); }

template<size_t N, typename T>
inline typename std::enable_if<
    is_unsigned_int<T>(),
    typename widest_int_t<T>::type
>::type
    bits_cast(T v)
{
    typedef
        typename widest_int_t<T>::type
        result_t;
    SYS_ASSERT((
        check_bits_cast<N, T>(v)));
    return
        static_cast<result_t>(v);
}

template<size_t N, typename T>
inline typename std::enable_if<
    is_unsigned_int<T>(),
bool>::type
    bits_cast(T v, typename widest_int_t<T>::type& r)
{
    typedef
        typename widest_int_t<T>::type
        result_t;
    if (check_bits_cast<N, T>(v)) {
        r = static_cast<result_t>(v);
        return true;
    }
    return false;
}

extern bool parse_num(
    const char* str, widest_sint_t& res, const char*& end);
extern bool parse_num(
    const char* str, widest_uint_t& res, const char*& end);

template<typename T>
inline typename std::enable_if<
    !is_widest_int<T>() &&
    is_integer<T>(),
bool>::type
    parse_num(const char* str, T& res, const char*& end)
{
    typedef
        typename widest_int_t<T>::type
        wider_t;
    wider_t val;
    if (!parse_num(str, val, end))
        return false;
    if (!integer_cast(val, res)) {
        errno = ERANGE;
        return false;
    }
    return true;
}

template<typename T>
inline bool
    parse_num(const char* str, T& res)
{
    const char* end;
    if (!parse_num(str, res, end))
        return false;
    if (*end) {
        errno = EINVAL;
        return false;
    }
    return true;
}

} // namespace Num

#endif /* __NUM_HPP */


