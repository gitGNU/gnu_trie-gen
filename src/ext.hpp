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

#ifndef __EXT_HPP
#define __EXT_HPP

#include <cctype>
#include <cstring>

#include <type_traits>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <functional>
#include <string>
#include <vector>
#include <memory>

#include "sys.hpp"
#include "num.hpp"

namespace Ext {

template<typename T>
constexpr bool is_ref()
{ return std::is_reference<T>::value; }

template<typename T>
constexpr bool is_const()
{ return std::is_const<T>::value; }

template<typename T>
constexpr bool is_const_ref()
{ return is_const<T>() && is_ref<T>(); }

template<typename B, typename D>
constexpr bool is_base_of()
{ return std::is_base_of<B, D>::value; }

template<typename T>
constexpr bool is_compound()
{ return std::is_compound<T>::value; }

template<typename T>
constexpr bool is_member_function_pointer()
{ return std::is_member_function_pointer<T>::value; }

// stev: Bjarne Stroustrup, TC++PL 4th edition,
//       34.3.1 unique_ptr, p. 990 (corrected)

template<typename T, typename... Args>
inline std::unique_ptr<T> make_unique(Args&&... args)
{ return std::unique_ptr<T>{new T{std::forward<Args>(args)...}}; }

// stev: a simple and useful wrapper class for arrays:
// it provides a range-checked  'operator[]'; the most
// relevant usage patterns are those of wrapping around
// the access to elements of statically defined arrays:
//
//   static void (*const funcs[])(std::ostream&) = {
//     f0, f1, f2
//   };
//   static void (*const funcs2[][3])(std::ostream&) = {{
//     f0, f1, f2
//   },{
//     f3, f4, f5
//   }};
//   array(funcs)[k](std::cout);
//   array(funcs2)[i][j](std::cout);
//
// Note that in the last two expression statements above
// the access to 'funcs' at index 'k' and, respectively,
// each of the accesses into 'funcs2' at indexes 'i' and
// subsequently 'j' gets range-checked in runtime.
//
// Another feature of this array class is that accesses
// to its elements via 'operator[]' member function are
// range checked against the underlying indexing type.
// For example, the access into 'funcs', with the index
// being the additive inverse of 'max<array_index_t>()'
// taken as 'long long', aborts the program:
//
//   const long long x = - static_cast<long long>(
//     Num::max<array_index_t>());
//   STATIC_ASSERT(static_cast<array_index_t>(x) == 1);
//   array(funcs)[x](std::cout);
//
// This is so due to an assertion failure in function
// 'integer_cast<array_index_t, long long>' which gets
// called in the one of the constructors of 'Num::box_t'.
// Should the type of the argument of 'operator[]' member
// function of class 'array_t' have instead been defined
// as 'array_index_t', by the implicit conversion rules
// the 'long long' 'x' would have silently been cast as
// the value '1' prior to the 'operator[]' function call
// -- which value, unfortunately, is a valid index into
// the 'funcs' array.
//
// Yet another feature of this array class having boxed
// indexing acceses to its elements -- feature which is
// derived from the functionality of 'integer_cast<R,V>'
// template function -- is that its indexing values can
// be enumerators:
//
//   enum struct E { w = 2, x };
//
//   array(funcs)[E::w](std::cout);
//   array(funcs)[E::x](std::cout);
//
// The first access into 'funcs' above is valid since
// the value 'E::w' would be boxed as the index value
// '2'. On the other hand, the second access into the
// array is aborting the program: boxing 'E::x' calls
// for 'integer_cast<array_index_t, E>(E::x)', which
// gets out the value '3', value which is out of the
// bounds of 'funcs' array.

// ISO/IEC 9899:1999(TC3) and 9899:2011
// 6.3 Conversions
// 6.3.1.1 Boolean, characters, and integers
// 6.3.1.3 Signed and unsigned integers
// 6.5.2.2 Function calls
// 6.5.2.2.7 (implicit argument conversions)

// ISO/IEC 14882:2003(E) and 14882:2011
// 4.5 Integral promotions
// 4.7 Integral conversions
// 5.2.2 Function call
// 5.2.2.7 (default argument promotions)
// 13.3.3.1 Implicit conversion sequences

// ISO/IEC 9899:1999(TC3) and 9899:2011
// 6.5.2.1 Array subscripting
// 6.5.2.1.3 (successive subscript operators)

// ISO/IEC 14882:2003(E) and 14882:2011
// 8.3.4 Arrays
// 8.3.4.7 (multidimensional arrays)

typedef unsigned int array_index_t;

template<typename T, array_index_t N>
class array_t final
{
public:
    typedef T elem_t;
    typedef T type_t[N];

    typedef Num::box_t<array_index_t> index_t;

    explicit array_t(type_t& _a) : a{_a} {}

    elem_t operator[](index_t i) const
    { SYS_ASSERT(i() < N); return a[i()]; }

private:
    elem_t* a;
};

template<typename T, array_index_t N, array_index_t M>
class array_t<T[M], N> final
{
public:
    typedef array_t<T, M> elem_t;
    typedef T type_t[N][M];

    typedef Num::box_t<array_index_t> index_t;

    explicit array_t(type_t& _a) : a{_a} {}

    elem_t operator[](index_t i) const
    { SYS_ASSERT(i() < N); return elem_t(a[i()]); }

private:
    typedef T inner_t[M];
    inner_t* a;
};

template<typename T, array_index_t N>
inline array_t<T, N> array(T (&v)[N])
{ return array_t<T, N>(v); }

template<typename T, array_index_t N>
constexpr array_index_t array_size(T (&v)[N] UNUSED)
{ return N; }

template <typename V>
class Cond;

template <typename V>
inline std::ostream& operator<<(
    std::ostream& ost, const Cond<V>& cond)
{
    cond.print(ost);
    return ost;
}

template<typename T>
struct array_to_pointer_t
{
    typedef
        typename std::conditional<
            Num::is_array<T>(),
            typename std::add_pointer<
                typename std::add_const<
                    typename std::remove_extent<T>::type
                    >::type
                >::type,
            T>::type
        type;
};

// stev: TODO: replace Cond classes with std::function instances

template <typename V>
class Cond
{
public:
    typedef
        typename array_to_pointer_t<V>::type
        value_t; //!!!HACKISH

    explicit Cond(
        bool _c,
        const V& _v) :
        c{_c}, v{_v}
    {}

    void print(std::ostream& ost) const
    { if (c) ost << v; }

private:
    bool c;
    value_t v;
};

template <typename V>
inline Cond<V> cond(bool c, const V& v)
{ return Cond<V>{c, v}; }

template <typename V1, typename V2>
class Cond2;

template <typename V1, typename V2>
inline std::ostream& operator<<(
    std::ostream& ost, const Cond2<V1, V2>& cond)
{
    cond.print(ost);
    return ost;
}

template <typename V1, typename V2>
class Cond2
{
public:
    typedef
        typename array_to_pointer_t<V1>::type
        value1_t; //!!!HACKISH
    typedef
        typename array_to_pointer_t<V2>::type
        value2_t; //!!!HACKISH

    explicit Cond2(
        bool _c,
        const V1& _v1,
        const V2& _v2) :
        c{_c}, v1{_v1}, v2{_v2}
    {}

    void print(std::ostream& ost) const
    { if (c) ost << v1; else ost << v2; }

private:
    bool     c;
    value1_t v1;
    value2_t v2;
};

template <typename V1, typename V2>
inline Cond2<V1, V2> cond(bool c, const V1& v1, const V2& v2)
{ return Cond2<V1, V2>{c, v1, v2}; }

typedef std::function<void (std::ostream&)> ostream_manip_t;

//!!!HACKISH
//inline std::ostream& operator<<(
//	std::ostream& ost, const ostream_manip_t& val)
//{ val(ost); return ost; }

template<typename F, typename... A>
inline typename std::enable_if<
    !is_member_function_pointer<F>(),
    ostream_manip_t>::type
manip(F&& func, A&&... args)
{
    using namespace std;
    return bind(
        forward<F>(func),
        placeholders::_1,
        forward<A>(args)...);
}

template<typename F, typename T, typename... A>
inline typename std::enable_if<
    is_member_function_pointer<F>(),
    ostream_manip_t>::type
manip(F&& func, T&& self, A&&... args)
{
    using namespace std;
    return bind(
        forward<F>(func),
        forward<T>(self),
        placeholders::_1,
        forward<A>(args)...);
}

template <typename T>
class Repr;

template <typename T>
inline std::ostream& operator<<(
    std::ostream& ost, const Repr<T>& repr)
{ repr.print(ost); return ost; }

template<typename T>
class ReprPrint
{
public:
    explicit ReprPrint(std::ostream& _ost) :
        ost(_ost) //!!!CURLY ost{_ost}
    {}

    size_t operator()(const T& val)
    {
        auto pos = ost.tellp();
        ost << val;
        return ost.tellp() - pos;
    }

private:
    std::ostream& ost;
};

template<>
class ReprPrint<double>
{
public:
    explicit ReprPrint(std::ostream& _ost) :
        ost(_ost) //!!!CURLY ost{_ost}
    {}

    size_t operator()(double val)
    {
        using namespace std;
        auto pos = ost.tellp();
        ost << fixed << setprecision(1) << val;
        return ost.tellp() - pos;
    }

private:
    std::ostream& ost;
};

enum struct char_repr_type_t {
    quoted,
    plain
};

enum struct str_repr_type_t {
    string,
    plain
};

template<>
class ReprPrint<char>
{
public:
    explicit ReprPrint(std::ostream& _ost) :
        ost(_ost) //!!!CURLY ost{_ost}
    {}

    size_t operator()(
        char ch, str_repr_type_t type)
    {
        const auto str =
            type == str_repr_type_t::string;

        switch (ch) {
        case '\\':
            return print_out_quoted('\\');
        case '\f':
            return print_out_quoted('f');
        case '\n':
            return print_out_quoted('n');
        case '\r':
            return print_out_quoted('r');
        case '\t':
            return print_out_quoted('t');
        case '\v':
            return print_out_quoted('v');
        //!!!REPR_SPACE
        //case ' ':
        //	return print_out_quoted(' ');
        case '"':
            if (str)
                return print_out_quoted('"');
            else
                return print_out(ch);
        case '\'':
            if (!str)
                return print_out_quoted('\'');
            else
                return print_out(ch);
        default:
            if (isascii(ch) && !iscntrl(ch))
                return print_out(ch);
            else
                return print_out_hex(ch, str);
        }
    }

    size_t operator()(
        char ch, char_repr_type_t type)
    {
        const auto q =
            type == char_repr_type_t::quoted;
        size_t n = 0;

        if (q)
            n += print_out('\'');

        n += operator()(ch, q
            ? str_repr_type_t::plain
            : str_repr_type_t::string);

        if (q)
            n += print_out('\'');

        return n;
    }

private:
    size_t print_out(char ch)
    { ost << ch; return 1; }

    size_t print_out_quoted(char ch)
    { ost << '\\' << ch; return 2; }

    size_t print_out_hex(char ch, bool str)
    {
        using namespace std;
        auto f = ost.fill();
        auto w = ost.width();
        auto m = ost.flags();
        auto c = static_cast<unsigned char>(ch);
        ost << "\\x"
            << hex
            << right
            << setfill('0')
            << setw(1 + str)
            << Num::integer_cast<unsigned>(c)
            << setfill(f)
            << setw(w);
        ost.flags(m);
        return 3 + (str || c >= '\x10');
    }

    std::ostream& ost;
};

template<>
struct ReprPrint<const char> : public ReprPrint<char>
{
    typedef ReprPrint<char> base_t;

    explicit ReprPrint(std::ostream& _ost) :
        base_t{_ost}
    {}
};

template<typename T>
class ReprPrint<T*>
{
public:
    explicit ReprPrint(std::ostream& _ost) :
        ost(_ost) //!!!CURLY {ost}
    {}

    size_t operator()(T* beg, T* end, bool str)
    {
        ReprPrint<T> repr{ost};
        if (str)
            ost << '"';
        size_t n = std::accumulate(
            beg, end, size_t(0),
            [&](size_t n, char ch) {
                return n + repr(
                    ch, str_repr_type_t::string);
            });
        if (str)
            ost << '"';
        if (str)
            n += 2;
        return n;
    }

private:
    std::ostream& ost;
};

template<typename T>
class Repr
{
public:
    explicit Repr(const T& _val) : val{_val} {}

    size_t print(std::ostream& ost) const
    { return ReprPrint<T>{ost}(val); }

private:
    typedef
        typename
            std::conditional<
                is_compound<T>(),
                const T&,
                T
            >::type
        val_t;

    val_t val;
};

template<>
class Repr<char>
{
public:
    Repr(char _ch, char_repr_type_t _type) :
        ch{_ch}, type{_type}
    {}

    size_t print(std::ostream& ost) const
    { return ReprPrint<char>{ost}(ch, type); }

private:
    char             ch;
    char_repr_type_t type;
};

template<typename T>
class Repr<T*>
{
public:
    Repr(const T* _beg, const T* _end, bool _str) :
        beg{_beg}, end{_end}, str{_str}
    {}

    Repr(const T* _beg, size_t _size, bool _str) :
        beg{_beg}, end{_beg + _size}, str{_str}
    {}

    size_t print(std::ostream& ost) const
    { return ReprPrint<const T*>{ost}(beg, end, str); }

private:
    const T *beg;
    const T *end;
    bool     str;
};

template<typename T>
inline typename std::enable_if<
    !Num::is_char_pointer<T>(),
Repr<T>>::type
repr(const T& val)
{ return Repr<T>{val}; }

template<typename T>
inline Repr<T*> repr(T* beg, T* end, bool str)
{ return Repr<T*>{beg, end, str}; }

template<typename T>
inline Repr<T*> repr(T* ptr, size_t size, bool str)
{ return Repr<T*>{ptr, size, str}; }

inline Repr<char> repr(char ch, bool quote = true)
{ return Repr<char>{
    ch, quote ? char_repr_type_t::quoted : char_repr_type_t::plain}; }

inline Repr<const char*> repr(const char* ptr, bool str = true)
{ return Repr<const char*>{ptr, ptr ? strlen(ptr) : 0, str}; }

inline Repr<const char*> repr(const std::string& obj, bool str = true)
{ return repr(obj.data(), obj.size(), str); }

inline Repr<const char*> repr(const std::vector<char>& obj, bool str = true)
{ return repr(obj.data(), obj.size(), str); }

} // namespace Ext

#endif /* __EXT_HPP */


