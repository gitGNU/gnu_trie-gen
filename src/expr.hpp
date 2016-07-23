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

#ifndef __EXPR_HPP
#define __EXPR_HPP

#include "glob.hpp"
#include "ext.hpp"

namespace Expr {

typedef global_options_t::expr_type_t expr_type_t;

class expr_def_t final
{
public:
    static const expr_def_t* get_def(expr_type_t);

    expr_def_t(const expr_def_t&) = delete;
    expr_def_t& operator=(const expr_def_t&) = delete;

    expr_def_t(expr_def_t&&) = delete;
    expr_def_t& operator=(expr_def_t&&) = delete;

    const char* const meta_chars;
    const char* const open_paren;
    const char* const close_paren;
    const char* const open_opt_paren;
    const char* const close_opt_paren;
    const char* const alter;
    bool        const sep;

private:
    expr_def_t(
        const char* _meta_chars,
        const char* _open_paren,
        const char* _close_paren,
        const char* _open_opt_paren,
        const char* _close_opt_paren,
        const char* _alter,
        bool        _sep) :
        meta_chars{_meta_chars},
        open_paren{_open_paren},
        close_paren{_close_paren},
        open_opt_paren{_open_opt_paren},
        close_opt_paren{_close_opt_paren},
        alter{_alter},
        sep{_sep}
    {}
    ~expr_def_t()
    {}
};

template<typename T>
class ReprPrint;

template<>
class ReprPrint<char>
{
public:
    explicit ReprPrint(
        std::ostream& _ost,
        const expr_def_t* _def,
        bool _spc) :
        ost(_ost),
        def(_def),
        spc(_spc)
    {}

    size_t operator()(char ch)
    {
        auto quo = 
            ch != '\\'
            && is_special_char(ch);
        if (quo)
            ost << '\\';
        return
            quo +
            Ext::ReprPrint<char>{ost}(ch,
                Ext::str_repr_type_t::string);
    }

private:
    static bool is_special_char(const char* set, char ch)
    { return set != nullptr && strchr(set, ch) != nullptr; }

    bool is_special_char(char ch) const
    {
        return
            (spc && ch == ' ') ||
            is_special_char(def->meta_chars, ch) ||
            is_special_char(globals->escape_chars, ch);
    }

    std::ostream& ost;
    const expr_def_t* def;
    bool spc;
};

template <typename T>
class Repr;

template <typename T>
inline std::ostream& operator<<(
    std::ostream& ost, const Repr<T>& repr)
{ repr.print(ost); return ost; }

template<>
struct ReprPrint<const char> :
    public ReprPrint<char>
{
    typedef ReprPrint<char> base_t;

    explicit ReprPrint(
        std::ostream& _ost,
        const expr_def_t* _def,
        bool _spc) :
        base_t{
            _ost,
            _def,
            _spc}
    {}
};

template<typename T>
class ReprPrint<T*>
{
public:
    explicit ReprPrint(
        std::ostream& _ost,
        const expr_def_t* _def,
        bool _spc) :
        ost(_ost),
        def(_def),
        spc(_spc)
    {}

    size_t operator()(T* beg, T* end)
    {
        ReprPrint<T> repr{ost, def, spc};
        return std::accumulate(
            beg, end, size_t(0),
            [&](size_t n, char ch)
                { return n + repr(ch); });
    }

private:
    std::ostream& ost;
    const expr_def_t* def;
    bool spc;
};

template<typename T>
class Repr
{
public:
    explicit Repr(
        const T& _val,
        const expr_def_t* _def,
        bool _spc) :
        val{_val},
        def{_def},
        spc{_spc}
    {}

    size_t print(std::ostream& ost) const
    { return ReprPrint<T>{ost, def, spc}(val); }

private:
    typedef
        typename
            std::conditional<
                Ext::is_compound<T>(),
                const T&,
                T
            >::type
        val_t;

    val_t             val;
    const expr_def_t* def;
    bool              spc;
};

template<>
class Repr<char>
{
public:
    Repr(char _ch, const expr_def_t* _def, bool _spc) :
        ch{_ch}, def{_def}, spc{_spc}
    {}

    size_t print(std::ostream& ost) const
    { return ReprPrint<char>{ost, def, spc}(ch); }

private:
    char              ch;
    const expr_def_t* def;
    bool              spc;
};

template<typename T>
class Repr<T*>
{
public:
    Repr(const T* _beg, const T* _end, const expr_def_t* _def, bool _spc) :
        beg{_beg}, end{_end}, def{_def}, spc{_spc}
    {}

    Repr(const T* _beg, size_t _size, const expr_def_t* _def, bool _spc) :
        beg{_beg}, end{_beg + _size}, def{_def}, spc{_spc}
    {}

    size_t print(std::ostream& ost) const
    { return ReprPrint<const T*>{ost, def, spc}(beg, end); }

private:
    const T*          beg;
    const T*          end;
    const expr_def_t* def;
    bool              spc;
};

template<typename T>
inline typename std::enable_if<
    !Num::is_char_pointer<T>(),
Repr<T>>::type
repr(const T& val, const expr_def_t* def, bool spc)
{ return Repr<T>{val, def, spc}; }

template<typename T>
inline Repr<T*> repr(T* beg, T* end, const expr_def_t* def, bool spc)
{ return Repr<T*>{beg, end, def, spc}; }

template<typename T>
inline Repr<T*> repr(T* ptr, size_t size, const expr_def_t* def, bool spc)
{ return Repr<T*>{ptr, size, def, spc}; }

inline Repr<char> repr(char ch, const expr_def_t* def, bool spc)
{ return Repr<char>{ch, def, spc}; }

inline Repr<const char*> repr(
    const char* ptr, const expr_def_t* def, bool spc)
{ return Repr<const char*>{ptr, ptr ? strlen(ptr) : 0, def, spc}; }

inline Repr<const char*> repr(
    const std::string& obj, const expr_def_t* def, bool spc)
{ return repr(obj.data(), obj.size(), def, spc); }

inline Repr<const char*> repr(
    const std::vector<char>& obj, const expr_def_t* def, bool spc)
{ return repr(obj.data(), obj.size(), def, spc); }

} // namespace Expr

#endif // _EXPR_HPP


