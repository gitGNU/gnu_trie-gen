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

#ifndef __TRIE_HPP
#define __TRIE_HPP

#include <cstdarg>

#include <type_traits>
#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <iterator>
#include <algorithm>
#include <functional>
#include <forward_list>

#include "glob.hpp"
#include "num.hpp"
#include "ext.hpp"
#include "expr.hpp"

#include "demangle.hpp"

// stev: enable the debugging code of
// ArrayTrie<...>::gen_compact_trie:
#define DEBUG_TRIE_ARRAY_COMPACT_TRIE

// stev: enable the debugging code of
// ArrayTrie<...>::gen_compact_code:
#define DEBUG_TRIE_ARRAY_COMPACT_CODE

// stev: enable the debugging code of
// ArrayTrie<...>::gen_wide_expr:
#define DEBUG_TRIE_ARRAY_WIDE_EXPR

// stev: enable the debugging code of
// ArrayTrie<...>::gen_compact_expr:
#define DEBUG_TRIE_ARRAY_COMPACT_EXPR

// stev: enable the debugging code of
// TernaryTrie<...>::gen_compact_code:
#define DEBUG_TRIE_TERNARY_COMPACT_CODE

// stev: enable the debugging code of
// TernaryTrie<...>::gen_wide_expr:
#define DEBUG_TRIE_TERNARY_WIDE_EXPR

// stev: enable the debugging code of
// TernaryTrie<...>::gen_compact_expr:
#define DEBUG_TRIE_TERNARY_COMPACT_EXPR

#ifdef DEBUG
# define DEBUG_CONDITION(N) (globals->debug >= (N))
#else
# define DEBUG_CONDITION(N) (false)
#endif

namespace Trie {

struct Error : public Sys::Error
{
    explicit Error(const char* what) :
        Sys::Error{what}
    {}
};

void error(const char* msg, ...)
    PRINTF_FMT(1);

void empty_key_error();

template<typename T>
void func_not_yet_impl(const char* func)
{
    error("%s: function %s not yet implemented",
        Sys::demangle<T>().str(),
        func);
}

template<typename C = char>
struct trie_traits_t
{
    typedef
        typename
            std::remove_cv<C>::type
        char_t;
    typedef
        typename
            std::make_unsigned<
                char_t
            >::type
        uchar_t;
    typedef
        std::char_traits<char_t>
        traits_t;
    typedef
        typename
            std::make_unsigned<
                typename traits_t
                ::int_type
            >::type
        uint_t;
    typedef
        std::basic_string<char_t>
        string_t;

    static const uint_t radix =
        Num::max<uchar_t>() + 1;

    static constexpr uint_t to_int(char_t ch)
    { return traits_t::to_int_type(ch); }

    static constexpr char_t to_char(uint_t uint)
    { return traits_t::to_char_type(uint); }

    static bool ult(char_t a, char_t b)
    {
        return
            static_cast<uchar_t>(a) <
            static_cast<uchar_t>(b);
    }

    static bool ugt(char_t a, char_t b)
    {
        return
            static_cast<uchar_t>(a) >
            static_cast<uchar_t>(b);
    }
};

// stev: Bjarne Stroustrup, TC++PL 4th edition,
// 34.3.1 unique_ptr, p. 987:
//
//    The uses of 'unique_ptr' include:
//    [...]
//    * storing pointers in containers
//    [...]

template<typename T>
class TriePool :
    private std::vector<std::unique_ptr<T>>
{
public:
    typedef std::vector<std::unique_ptr<T>> base_t;
    typedef T node_t;

    TriePool()  {}
    ~TriePool() {}

    TriePool(const TriePool&) = delete;
    TriePool& operator=(const TriePool&) = delete;

    // stev: we might allow moving the pool;
    //       for the time being this cut-off
    //       functionality is sufficient
    TriePool(TriePool&&) = delete;
    TriePool operator=(TriePool&&) = delete;

    node_t* new_node() noexcept
    {
        // stev: assert the existence of a moving
        // 'push_back' member function in base_t
        using
            moving_func_t =
            void (base_t::*)(typename base_t::value_type&&);
        STATIC_ASSERT(
            static_cast<moving_func_t>(&base_t::push_back));
        // stev: terminate the program when
        //       failed allocating a new node
        auto n = Ext::make_unique<node_t>();
        // stev: retain the newly created pointer
        //       such that we can pass it out
        auto r = n.get();
        // stev: move the unique pointer into the
        //       pool: the lifetime of the pointer
        //       extends until that of the pool
        base_t::push_back(std::move(n));
        return r;
    }

    using base_t::size;

    void stat(
        std::ostream& ost,
        const char* head = nullptr) const;
};

template<typename T>
void TriePool<T>::stat(
    std::ostream& ost, const char* head) const
{
    auto n = node_t::size();
    auto p = base_t::size();
    if (head == nullptr)
        head = "#";
    ost << head << " node size:  " << n << '\n'
        << head << " node count: " << p << '\n'
        << head << " nodes size: " << p * n << '\n';
}

template<typename T>
struct node_attrs_t
{
    typedef typename T::char_t char_t;
    typedef std::vector<char_t> path_t;

    node_attrs_t() = default;

    node_attrs_t(
        size_t _width,
        char_t _last = 0) :
        width{_width},
        last{_last}
    {}

    operator bool() const
    { return width; }

    // stev: both 'operator+=' functions below
    // are defined for convenience of notation
    // of updating the class members.

    void operator+=(size_t w)
    { width += w; }

    void operator+=(const node_attrs_t& a)
    {
        width += a.width;
        if (T::ult(last, a.last))
            last = a.last;
    }

    friend
    std::ostream& operator<<(
        std::ostream& ost, const node_attrs_t& at)
    {
        using namespace Ext;
        ost << '['
            << at.width
            << cond(at.last, ',')
            << cond(at.last, repr(at.last))
            << ']';
        return ost;
    }

    size_t width = 0;
    char_t last = 0;
};

template<typename T>
inline
Ext::ostream_manip_t print_attrs(
    const node_attrs_t<T>& attrs,
    typename node_attrs_t<T>::char_t ch,
    const typename node_attrs_t<T>::path_t& path,
    size_t start)
{
    // stev: TODO: figure out (reading the C++11
    // standard and TC++PL 4/e) whether the type
    // of the inner 'path' (the one implicitely
    // defined inside the lambda function below)
    // is 'path_t' or 'const path_t&'! The former
    // case implies an extra copy of the outer
    // 'path' -- which we do not want to happen.
    //
    // stev: Pragmatic resolution:
    // Inserting into the output sequence below
    // someting like:
    //
    //   << cond(is_const<typeof(path)>(), "const")
    //   << cond(is_ref<decltype(path)>(), '&', '=')
    //
    // the program shows -- at least in case of
    // compiling it with GCC 4.8.0 -- that the
    // type of the inner 'path' is a const ref.

    using namespace Ext;
    //std::cerr
    //	<< "\n!!! "
    //	<< cond(is_const<typeof(path)>(), "const")
    //	<< cond(is_ref<decltype(path)>(), '&', '=')
    //	<< std::endl;
    return [=](std::ostream& ost) {
        ost << '['
            << start
            << ','
            //<< cond(is_const<typeof(path)>(), "const")
            //<< cond(is_ref<decltype(path)>(), '&', '=')
            << repr(path)
            << ','
            << repr(ch)
            << ','
            << attrs
            << ']';
    };
}

enum class gen_type_t {
    compact,
    wide
};

// stev: Knuth, TAOCP, vol 3, 3rd edition,
// 6.3 Digital Searching

template<
    typename T,
    typename C = char,
    template<typename> class R = trie_traits_t>
class ArrayTrie :
    private R<C>
{
public:
    typedef T value_t;
    typedef R<C> traits_t;
    typedef typename traits_t::char_t char_t;
    typedef typename traits_t::string_t string_t;

    ArrayTrie()
    { root = pool.new_node(); }
    ~ArrayTrie()
    {}

    // stev: for the time being copy and move ops
    //       of ArrayTrie are deleted; we might well
    //       implement these when needed, sometime
    //       in the future

    ArrayTrie(const ArrayTrie&) = delete;
    ArrayTrie& operator=(const ArrayTrie&) = delete;

    ArrayTrie(ArrayTrie&&) = delete;
    ArrayTrie operator=(ArrayTrie&&) = delete;

    const value_t* get(const string_t& key) const
    { return get(key.data()); }

    const value_t* get(const char_t* key) const
    { return try_get_value(lookup(key)); }

    void put(const string_t& key, const value_t& val = {})
    { put(key.data(), val); }

    void put(const char_t* key, const value_t& val = {})
    { insert(key)->value = make_value(val); }

    void put(const string_t& key, value_t&& val)
    { put(key.data(), std::move(val)); }

    void put(const char_t* key, value_t&& val)
    { insert(key)->value = make_value(std::move(val)); }

    void stat(
        std::ostream& ost,
        const char* head = nullptr) const
    { pool.stat(ost, head); }

    void print(std::ostream& ost) const;

    void gen_trie(std::ostream& ost, gen_type_t type) const;
    void gen_code(std::ostream& ost, gen_type_t type) const;
    void gen_expr(std::ostream& ost, gen_type_t type) const;

    friend
    std::ostream& operator<<(
        std::ostream& ost, const ArrayTrie& trie)
    { trie.print(ost); return ost; }

private:
    struct node_t;

    typedef TriePool<node_t> pool_t;

    typedef Trie::node_attrs_t<traits_t> node_attrs_t;
    typedef std::tuple<char_t, node_t*> node_elem_t;
    typedef std::tuple<bool, value_t> node_value_t;
    typedef typename node_attrs_t::path_t path_t;
    typedef Expr::expr_def_t expr_def_t;

    using traits_t::radix;
    using traits_t::to_int;
    using traits_t::to_char;

    static node_elem_t make_elem(node_t* node, char_t ch = 0)
    { return std::make_tuple(ch, node); }

    static node_value_t make_value(const value_t& val)
    { return std::make_tuple(true, val); }
    static node_value_t make_value(value_t&& val)
    { return std::make_tuple(true, std::move(val)); }

    static bool has_value(node_t* n)
    { return std::get<0>(n->value); }

    static value_t& get_value(node_t* n)
    { return std::get<1>(n->value); }

    static value_t* try_get_value(node_t* n)
    { return n && has_value(n) ? &get_value(n) : nullptr; }

    static void print_obj(
        std::ostream& ost, const node_elem_t elem,
        size_t level = 0);
    static void gen_wide_trie(
        std::ostream& ost, const node_elem_t elem,
        size_t level = 0);
    static void gen_compact_trie(
        std::ostream& ost, const node_elem_t elem,
        path_t& path, size_t start = 0,
        size_t level = 1);
    static void gen_wide_code(
        std::ostream& ost, const node_elem_t elem,
        const node_t* parent = 0,
        size_t level = 0);
    static void gen_compact_code(
        std::ostream& ost, const node_elem_t elem,
        path_t& path, const node_attrs_t& attrs,
        size_t start = 0,
        size_t level = 1);
    static void gen_wide_expr(
        std::ostream& ost, const node_elem_t elem,
        path_t& path, const expr_def_t* def,
        size_t start = 0,
        size_t level = 1);
    static void gen_compact_expr(
        std::ostream& ost, const node_elem_t elem,
        path_t& path, const expr_def_t* def,
        size_t start = 0,
        size_t level = 1);

    void check_key(const char_t* key) const;

    node_t* lookup(const char_t* key) const;
    node_t* insert(const char_t* key);

    node_t* root = nullptr;
    pool_t  pool;
};

template<
    typename T,
    typename C,
    template<typename> class R>
struct ArrayTrie<T, C, R>::node_t
{
    typedef ArrayTrie<T, C, R> trie_t;
    typedef trie_t::node_elem_t elem_t;
    typedef trie_t::node_value_t value_t;
    typedef trie_t::node_attrs_t attrs_t;
    typedef std::array<node_t*, radix> child_t;

    node_t() noexcept
    { child.fill(nullptr); }

    ~node_t() noexcept
    {}

    node_t*& next(char_t key);

    class const_iterator;

    const_iterator cbegin() const;
    const_iterator cend() const;

    static constexpr size_t size()
    { return sizeof(node_t); }

    friend const_iterator begin(const node_t& node)
    { return node.cbegin(); }
    friend const_iterator end(const node_t& node)
    { return node.cend(); }

    value_t value;
    child_t child;

    // stev: node annotation attributes
    // necessary for code generation
    attrs_t attrs;
};

template<
    typename T,
    typename C,
    template<typename> class R>
class ArrayTrie<T, C, R>::node_t::const_iterator :
    public std::iterator<
        std::input_iterator_tag,
        elem_t,
        std::ptrdiff_t,
        elem_t,
        elem_t>
{
public:
    const_iterator& operator++()
    { ptr = next(beg, ++ ptr); return *this; }

    const_iterator operator++(int)
    { const_iterator r = *this; operator++(); return r; }

    elem_t operator*() const
    { return make_elem(); }
    elem_t operator->() const
    { return make_elem(); }

    bool operator==(const const_iterator& v) const
    { return ptr == v.ptr; }
    bool operator!=(const const_iterator& v) const
    { return ptr != v.ptr; }

protected:
    friend class node_t;

    typedef
        typename node_t::child_t::const_iterator
        child_iterator;

    const_iterator(
        const child_iterator& _beg,
        const child_iterator& _ptr) :
        beg{_beg}, ptr{_ptr}
    {}

    static const_iterator make_begin(const child_iterator& beg)
    { return const_iterator{beg, next(beg, beg)}; }

    static const_iterator make_end(const child_iterator& beg)
    { return const_iterator{beg, beg + radix}; }

    static child_iterator next(
        const child_iterator& beg, const child_iterator& ptr)
    {
        return std::find_if(
            ptr, beg + radix,
            [](const node_t* n) { return n != nullptr; });
    }

    elem_t make_elem() const
    { return std::make_tuple(to_char(ptr - beg), *ptr); }

    child_iterator beg;
    child_iterator ptr;
};

template<
    typename T,
    typename C,
    template<typename> class R>
inline typename
ArrayTrie<T, C, R>::node_t::const_iterator
ArrayTrie<T, C, R>::node_t::cbegin() const
{ return const_iterator::make_begin(child.cbegin()); }

template<
    typename T,
    typename C,
    template<typename> class R>
inline typename
ArrayTrie<T, C, R>::node_t::const_iterator
ArrayTrie<T, C, R>::node_t::cend() const
{ return const_iterator::make_end(child.cbegin()); }

template<
    typename T,
    typename C,
    template<typename> class R>
inline typename
ArrayTrie<T, C, R>::node_t*&
ArrayTrie<T, C, R>::node_t::next(char_t key)
{ return child.at(to_int(key)); }

template<
    typename T,
    typename C,
    template<typename> class R>
void ArrayTrie<T, C, R>::check_key(
    const char_t* key) const
{
    if (*key == 0)
        empty_key_error();
}

template<
    typename T,
    typename C,
    template<typename> class R>
typename
ArrayTrie<T, C, R>::node_t*
ArrayTrie<T, C, R>::lookup(const char_t* key) const
{
    check_key(key);
    SYS_ASSERT(root != nullptr);
    SYS_ASSERT(*key);

    auto node = root;
    do node = node->next(*key ++);
    while (node && *key);

    return node;
}

template<
    typename T,
    typename C,
    template<typename> class R>
typename
ArrayTrie<T, C, R>::node_t*
ArrayTrie<T, C, R>::insert(const char_t* key)
{
    check_key(key);
    SYS_ASSERT(root != nullptr);
    SYS_ASSERT(*key);

    node_t* node = root;
    node_t* prev = nullptr;
    do node = (prev = node)->next(*key ++);
    while (node && *key);

    if (node)
        return node;

    key --;
    node = prev;
    while (*key) {
        node->attrs += node_attrs_t{1, *key};
        node = node->next(*key ++) = pool.new_node();
    }

    return node;
}

//!!!HACKISH
inline std::ostream& operator<<( //!!!HACKISH
    std::ostream& ost, const Ext::ostream_manip_t& val)
{ val(ost); return ost; }

inline Ext::ostream_manip_t indent(size_t level)
{
    static const auto dots = [](
        std::ostream& ost, size_t level)
    {
        while (level --)
            ost << std::setw(1) << '.'
                << std::setw(3) << ' ';
    };
    static const auto spaces = [](
        std::ostream& ost, size_t level)
    {
        if (level)
            ost << std::setw(4 * level) << ' ';
    };

    return Ext::manip(
        globals->print_dots ? dots : spaces,
        level);
}

template<
    typename T,
    typename C,
    template<typename> class R>
void ArrayTrie<T, C, R>::print_obj(
    std::ostream& ost, const node_elem_t elem,
    size_t level)
{
    using namespace Ext;
    using std::setw;
    using std::get;

    const auto ch = get<0>(elem);
    const auto node = get<1>(elem);
    SYS_ASSERT(node != nullptr);

    if (level)
        ost << indent(level)
            << repr(ch);

    auto at = has_value(node);
    if (at)
        ost << ": "
            << repr(get_value(node));

    if (globals->print_attributes && node->attrs)
        ost << cond(at || level, ' ')
            << node->attrs
            << cond(!level, ' ');

    size_t k = 0;
    for (const auto e: *node) {
        if (k ++ == 0) ost
            << cond(at || level, ' ')
            << "{\n";
        print_obj(ost, e, level + 1);
    }
    if (k)
        ost << indent(level)
            << setw(1)
            << "}\n";
    else
    if (level)
        ost << '\n';
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline void ArrayTrie<T, C, R>::print(
    std::ostream& ost) const
{
    SYS_ASSERT(root != nullptr);
    print_obj(ost, make_elem(root));
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline void ArrayTrie<T, C, R>::gen_wide_trie(
    std::ostream& ost, const node_elem_t elem,
    size_t level)
{ print_obj(ost, elem, level); }

template<
    typename T,
    typename C,
    template<typename> class R>
void ArrayTrie<T, C, R>::gen_compact_trie(
    std::ostream& ost, const node_elem_t elem,
    path_t& path, size_t start, size_t level)
{
    using namespace Ext;
    using std::get;

    const auto ch = get<0>(elem);
    const auto node = get<1>(elem);
    SYS_ASSERT(node != nullptr);

    const auto n =
        node->attrs.width;
    const auto child =
        has_value(node);
    const auto at =
        ch && !child && n == 1;
    const auto obj =
        ch && n && !at;

    const auto pd = path.data();
    const auto ps = path.size();
    SYS_ASSERT(ps >= start);

#if defined(DEBUG) && \
    defined(DEBUG_TRIE_ARRAY_COMPACT_TRIE)
    const auto dump_vars = [&](std::ostream& ost) {
        ost << indent(level)
            << "//"
            << " ch=" << repr(ch)
            << " path=" << repr(path)
            << " start=" << start
            << " n=" << n
            << " child=" << child
            << " at=" << at
            << " obj=" << obj
            << " ps=" << ps
            << '\n';
    };
    if (globals->debug > 1)
        dump_vars(ost);
#endif

    if (at)
        path.push_back(ch);

    // stev: !at <=> child || n != 1;
    //       therefore: child => !at
    if (!at && ch)
        ost << indent(level)
            << '"'
            << repr(pd + start, pd + ps, false)
            << repr(ch, false)
            << '"';

    if (!at && ch && globals->print_attributes)
        ost << ' '
            << print_attrs(
                node->attrs, ch,
                path, start);

    // stev: child => !at (see above)
    if (child)
        ost << ": "
            << repr(get_value(node));

    // stev: obj => !at (see above)
    if (obj)
        ost << " {";

    if (obj || child)
        ost << '\n';

    for (const auto e: *node)
        gen_compact_trie(
            ost, e, path,
            !at ? ps : start,
            level + obj);

    if (obj)
        ost << indent(level)
            << "}\n";

    if (at)
        path.pop_back();
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline void ArrayTrie<T, C, R>::gen_trie(
    std::ostream& ost,
    gen_type_t type)
    const
{
    SYS_ASSERT(root != nullptr);

    if (type == gen_type_t::compact) {
        if (root->attrs) {
            if (globals->print_attributes)
                ost << root->attrs
                    << ' ';
            ost << "{\n";
        }

        path_t path;
        gen_compact_trie(
            ost, make_elem(root), path);

        if (root->attrs)
            ost << "}\n";
    }
    else
    if (type == gen_type_t::wide)
        gen_wide_trie(
            ost, make_elem(root));
    else
        SYS_UNEXPECT_ERR(
            "unknown gen-type: %zu",
            Num::integer_cast<size_t>(
                type));
}

template<
    typename T,
    typename C,
    template<typename> class R>
void ArrayTrie<T, C, R>::gen_wide_code(
    std::ostream& ost, const node_elem_t elem,
    const node_t* parent, size_t level)
{
    using namespace Ext;
    using std::setw;
    using std::get;

    const auto ch = get<0>(elem);
    const auto node = get<1>(elem);
    SYS_ASSERT(node != nullptr);

    if (globals->print_attributes)
        ost << indent(level + !parent)
            << "// ";
    if (globals->print_attributes && parent)
        ost << repr(ch)
            << ' ';
    if (globals->print_attributes)
        ost << node->attrs
            << '\n';

    if (parent)
        ost << indent(level)
            << cond(parent->attrs.width > 1,
                "case", "if (*p ++ ==")
            << ' '
            << repr(ch)
            << cond(parent->attrs.width > 1,
                ':', ") {")
            << '\n';

    if (has_value(node))
        ost << indent(level + 1)
            << "if (*p == 0)\n"
            << indent(level + 2)
            << "return "
            << repr(get_value(node))
            << ";\n";

    if (node->attrs.width > 1)
        ost << indent(level + 1)
            << "switch (*p ++) {\n";

    for (const auto e: *node)
        gen_wide_code(ost, e, node, level + 1);

    // stev: note that the closing curly
    // brace coresponding to that printed
    // under the condition:
    //
    //  'parent && parent->attrs.width <= 1'
    //
    // is printed by the output statement
    // that follows below under condition:
    //
    //  'node->attrs.width <= 1'
    //
    // during the next recursive call to the
    // function: then the inner 'parent' is
    // set to be the outer 'node'.
    if (node->attrs.width)
        ost << indent(level + 1)
            << "}\n";

    if (parent == nullptr ||
        parent->attrs.last != ch)
        ost << indent(level + 1)
            << "return error;\n";
}

template<
    typename T,
    typename C,
    template<typename> class R>
void ArrayTrie<T, C, R>::gen_compact_code(
    std::ostream& ost, const node_elem_t elem,
    path_t& path, const node_attrs_t& attrs,
    size_t start, size_t level)
{
    using namespace Ext;
    using std::get;

    const auto ch = get<0>(elem);
    const auto node = get<1>(elem);
    SYS_ASSERT(node != nullptr);

    const auto n =
        node->attrs.width;
    const auto child =
        has_value(node);
    const auto at =
        ch && !child && n == 1;
    const auto obj =
        n && !at; //!!! ch && n && !at

    const auto pd = path.data();
    const auto ps = path.size();
    SYS_ASSERT(ps >= start);
    const size_t l = ps - start;

#if defined(DEBUG) && \
    defined(DEBUG_TRIE_ARRAY_COMPACT_CODE)
    const auto dump_vars = [&](std::ostream& ost) {
        ost << indent(level)
            << "//"
            << " path=" << repr(path)
            << " attrs=" << attrs << ',' << node->attrs
            << " start=" << start
            << " ch=" << repr(ch)
            << " n=" << n
            << " child=" << bool(child)
            << " at=" << at
            << " obj=" << obj
            << " ps=" << ps
            << " l=" << l
            << '\n';
    };
    if (globals->debug > 1)
        ost << dump_vars;
#endif

    if (at)
        path.push_back(ch);

    if (!at && globals->print_attributes) {
        ost << indent(level)
            << "// ";

        if (!ch)
            ost << node->attrs;
        else
            ost << print_attrs(
                    node->attrs, ch,
                    path, start);

        ost << '\n';
    }

    const auto b = l > 0;
    const auto c =
        !child || node->attrs.width;
    const auto w = attrs.width > 1;
    const auto s = start + (w && b);
    const auto t = !w || b;
    const auto i = t || s < ps;
    const auto j = c && i;
    const auto d = bool(ch) + (w && b); //!!!CH
    const auto a = c || !i;

#if defined(DEBUG) && \
    defined(DEBUG_TRIE_ARRAY_COMPACT_CODE)
    const auto dump_vars2 = [&](std::ostream& ost) {
        ost << indent(level)
            << "//"
            << " attrs=" << attrs << ',' << node->attrs
            << " b=" << b
            << " c=" << c
            << " w=" << w
            << " s=" << s
            << " t=" << t
            << " i=" << i
            << " j=" << j
            << " d=" << d
            << " a=" << a
            << '\n';
    };
#endif
    // stev: !at <=> !ch || child || n != 1;
    //       therefore: child => !at
    if (!at && ch) {
        if (w)
            ost << indent(level)
                << "case "
                << repr(b ? pd[start] : ch)
                << ":\n";
        if (i)
            ost << indent(level + w)
                << "if (";
        for (auto p = pd + s, e = pd + ps;
            p < e; ++ p) {
            ost << "*p ++ == "
                << repr(*p);
            if (p + 1 < e || t)
                ost << " &&\n"
                    << indent(level + w + 1);
        }
        if (t)
            ost << "*p ++ == "
                << repr(ch);
        if (j)
            ost << ") {\n";
    }

    // stev: child => !at (see above)
    if (child) {
#if defined(DEBUG) && \
    defined(DEBUG_TRIE_ARRAY_COMPACT_CODE)
        if (globals->debug > 1)
            ost << cond(!a, '\n')
                << dump_vars
                << dump_vars2;
#endif
        if (a)
            ost << indent(level + d)
                << "if (";
        else
            ost << " &&\n"
                << indent(level + d);

        ost << "*p == 0)\n"
            << indent(level + d + a)
            << "return "
            << repr(get_value(node))
            << ";\n";
    }

    if (obj && node->attrs.width > 1)
        ost << indent(level + d)
            << "switch (*p ++) {\n";

    for (const auto e: *node)
        gen_compact_code(
            ost, e, path,
            obj ? node->attrs : attrs,
            !at ? ps : start,
            level + (obj ? d : 0)); //!!! + obj);

    if ((obj && node->attrs.width > 1) ||
        DEBUG_CONDITION(8)) {
#if defined(DEBUG) && \
    defined(DEBUG_TRIE_ARRAY_COMPACT_CODE)
        if (globals->debug > 1)
            ost << dump_vars
                << dump_vars2;
#endif
        ost << indent(level + d)
            << "}\n";
    }

    if ((!at && ch && j) ||
        DEBUG_CONDITION(8)) {
#if defined(DEBUG) && \
    defined(DEBUG_TRIE_ARRAY_COMPACT_CODE)
        if (globals->debug > 1)
            ost << dump_vars
                << dump_vars2;
#endif
        ost << indent(level + w)
            << "}\n";
    }

    if (((attrs.last != ch && !l) || !ch) || //!!!CH || !ch
        DEBUG_CONDITION(9)) {
#if defined(DEBUG) && \
    defined(DEBUG_TRIE_ARRAY_COMPACT_CODE)
        if (globals->debug > 1)
            ost << dump_vars;
#endif
        ost << indent(level + bool(ch)) //!!!CH
            << "return error;\n";
    }

    if (at)
        path.pop_back();
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline void ArrayTrie<T, C, R>::gen_code(
    std::ostream& ost,
    gen_type_t type)
    const
{
    SYS_ASSERT(root != nullptr);

    if (type == gen_type_t::compact) {
        path_t path;
        gen_compact_code(
            ost, make_elem(root),
            path, root->attrs);
    }
    else
    if (type == gen_type_t::wide)
        gen_wide_code(
            ost, make_elem(root));
    else
        SYS_UNEXPECT_ERR(
            "unknown gen-type: %zu",
            Num::integer_cast<size_t>(
                type));
}

template<
    typename T,
    typename C,
    template<typename> class R>
void ArrayTrie<T, C, R>::gen_wide_expr(
    std::ostream& ost, const node_elem_t elem,
    path_t& path, const expr_def_t* def,
    size_t start, size_t level)
{
    using namespace Ext;
    using std::get;

    const auto ch = get<0>(elem);
    const auto node = get<1>(elem);
    SYS_ASSERT(node != nullptr);

    SYS_ASSERT(
        globals->expr_depth == 0 ||
        globals->expr_depth >= level);

    const auto n =
        node->attrs.width;
    const auto child =
        has_value(node);
    const auto at =
        ch && !child && n == 1;
    const auto obj =
        ch && n && !at;

    const auto pd = path.data();
    const auto ps = path.size();
    SYS_ASSERT(ps >= start);

#if defined(DEBUG) && \
    defined(DEBUG_TRIE_ARRAY_WIDE_EXPR)
    const auto dump_vars = [&](std::ostream& ost) {
        ost << indent(level)
            << "//"
            << " ch=" << repr(ch)
            << " path=" << repr(path)
            << " start=" << start
            << " n=" << n
            << " child=" << child
            << " at=" << at
            << " obj=" << obj
            << " ps=" << ps
            << '\n';
    };
    if (globals->debug > 1)
        dump_vars(ost);
#endif

    if (at)
        path.push_back(ch);

    // stev: !at <=> child || n != 1;
    //       therefore: child => !at
    if (!at && ch) {
        using namespace Expr;
        ost << '\n' << indent(level)
            << repr(pd + start, pd + ps, def, true)
            << repr(ch, def, true);
    }

    if (!at && ch && globals->print_attributes)
        ost << ' '
            << print_attrs(
                node->attrs, ch,
                path, start);

    if (globals->expr_depth == 0 ||
        globals->expr_depth >= level + obj) {
        // stev: obj => !at (see above)
        if (obj)
            ost << ' '
                << (child
                    ? def->open_opt_paren
                    : def->open_paren);

        size_t k = 0;
        for (const auto e: *node) {
            if (k ++)
                ost << ' '
                    << def->alter;
            gen_wide_expr(
                ost, e, path, def,
                !at ? ps : start,
                level + obj);
        }

        if (obj)
            ost << '\n' << indent(level)
                << (child
                    ? def->close_opt_paren
                    : def->close_paren);
    }

    if (at)
        path.pop_back();
}

template<
    typename T,
    typename C,
    template<typename> class R>
void ArrayTrie<T, C, R>::gen_compact_expr(
    std::ostream& ost, const node_elem_t elem,
    path_t& path, const expr_def_t* def,
    size_t start, size_t level)
{
    using namespace Ext;
    using std::get;

    const auto ch = get<0>(elem);
    const auto node = get<1>(elem);
    SYS_ASSERT(node != nullptr);

    SYS_ASSERT(
        globals->expr_depth == 0 ||
        globals->expr_depth >= level);

    const auto n =
        node->attrs.width;
    const auto child =
        has_value(node);
    const auto at =
        ch && !child && n == 1;
    const auto obj =
        ch && n && !at;

    const auto pd = path.data();
    const auto ps = path.size();
    SYS_ASSERT(ps >= start);

#if defined(DEBUG) && \
    defined(DEBUG_TRIE_ARRAY_COMPACT_EXPR)
    const auto dump_vars = [&](std::ostream& ost) {
        ost << indent(level)
            << "//"
            << " ch=" << repr(ch)
            << " path=" << repr(path)
            << " start=" << start
            << " n=" << n
            << " child=" << child
            << " at=" << at
            << " obj=" << obj
            << " ps=" << ps
            << '\n';
    };
    if (globals->debug > 1)
        dump_vars(ost);
#endif

    if (at)
        path.push_back(ch);

    // stev: !at <=> child || n != 1;
    //       therefore: child => !at
    if (!at && ch) {
        using namespace Expr;
        ost << repr(pd + start, pd + ps, def, false)
            << repr(ch, def, false);
    }

    if (!at && ch && globals->print_attributes)
        ost << ' '
            << print_attrs(
                node->attrs, ch,
                path, start);

    if (globals->expr_depth == 0 ||
        globals->expr_depth >= level + obj) {
        // stev: obj => !at (see above)
        if (obj)
            ost << (child
                    ? def->open_opt_paren
                    : def->open_paren);;

        size_t k = 0;
        for (const auto e: *node) {
            if (k ++)
                ost << def->alter;
            gen_compact_expr(
                ost, e, path, def,
                !at ? ps : start,
                level + obj);
        }

        if (obj)
            ost<< (child
                    ? def->close_opt_paren
                    : def->close_paren);
    }

    if (at)
        path.pop_back();
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline void ArrayTrie<T, C, R>::gen_expr(
    std::ostream& ost,
    gen_type_t type)
    const
{
    SYS_ASSERT(root != nullptr);

    const auto wide = type == gen_type_t::wide;
    const auto bash = globals->expr_type ==
        Expr::expr_type_t::bash;

    if (wide) {
        if (root->attrs &&
            globals->print_attributes)
            ost << root->attrs
                << ' ';
    }
    else
    if (type != gen_type_t::compact)
        SYS_UNEXPECT_ERR(
            "unknown gen-type: %zu",
            Num::integer_cast<size_t>(
                type));

    path_t path;
    auto def = expr_def_t::get_def(globals->expr_type);

    using namespace Ext;
    ost << cond(wide || bash, def->open_paren);

    if (wide)
        gen_wide_expr(
            ost, make_elem(root), path, def);
    else
        gen_compact_expr(
            ost, make_elem(root), path, def);

    ost << cond(wide, '\n')
        << cond(wide || bash, def->close_paren)
        << '\n';
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline Ext::ostream_manip_t print(
    const ArrayTrie<T, C, R>& trie)
{
    typedef
        ArrayTrie<T, C, R> trie_t;
    return
        Ext::manip(&trie_t::print, &trie);
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline Ext::ostream_manip_t gen_trie(
    const ArrayTrie<T, C, R>& trie, gen_type_t type)
{
    typedef
        ArrayTrie<T, C, R> trie_t;
    return
        Ext::manip(&trie_t::gen_trie, &trie, type);
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline Ext::ostream_manip_t gen_code(
    const ArrayTrie<T, C, R>& trie, gen_type_t type)
{
    typedef
        ArrayTrie<T, C, R> trie_t;
    return
        Ext::manip(&trie_t::gen_code, &trie, type);
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline Ext::ostream_manip_t gen_expr(
    const ArrayTrie<T, C, R>& trie, gen_type_t type)
{
    typedef
        ArrayTrie<T, C, R> trie_t;
    return
        Ext::manip(&trie_t::gen_expr, &trie, type);
}

// stev: http://www.cs.princeton.edu/~rs/strings/
//
// Bentley and Sedgewick: Ternary Search Trees
// Dr. Dobbs Journal April, 1998
// http://www.drdobbs.com/database/ternary-search-trees/184410528
//
// Bentley and Sedgewick: Fast Algorithms for Sorting
//   and Searching Strings
// SODA '97: Proceedings of the eighth annual ACM-SIAM
//   symposium on Discrete algorithms,
//   pp. 360-369, 1997
// http://www.cs.princeton.edu/~rs/strings/paper.pdf

template<
    typename T,
    typename C = char,
    template<typename> class R = trie_traits_t>
class TernaryTrie :
    private R<C>
{
public:
    typedef T value_t;
    typedef R<C> traits_t;
    typedef typename traits_t::char_t char_t;
    typedef typename traits_t::string_t string_t;

    TernaryTrie()
    {}
    ~TernaryTrie()
    {}

    // stev: for the time being we disallow copy and
    //       move ops of TernaryTrie; we should better
    //       implement these sometime in the future

    TernaryTrie(const TernaryTrie&) = delete;
    TernaryTrie& operator=(const TernaryTrie&) = delete;

    TernaryTrie(TernaryTrie&&) = delete;
    TernaryTrie operator=(TernaryTrie&&) = delete;

    const value_t* get(const string_t& key) const
    { return get(key.data()); }

    const value_t* get(const char_t* key) const
    { auto n = lookup(key); return n ? n->value() : nullptr; }

    void put(const string_t& key, const value_t& val = {})
    { put(key.data(), val); }

    void put(const char_t* key, const value_t& val = {})
    { insert(key)->value(make_value(val)); }

    void put(const string_t& key, value_t&& val)
    { put(key.data(), std::move(val)); }

    void put(const char_t* key, value_t&& val)
    { insert(key)->value(make_value(std::move(val))); }

    void stat(
        std::ostream& ost,
        const char* head = nullptr) const
    { pool.stat(ost, head); }

    void print(std::ostream& ost) const;

    void gen_trie(std::ostream& ost, gen_type_t type) const;
    void gen_code(std::ostream& ost, gen_type_t type) const;
    void gen_expr(std::ostream& ost, gen_type_t type) const;

    friend
    std::ostream& operator<<(
        std::ostream& ost, const TernaryTrie& trie)
    { trie.print(ost); return ost; }

private:
    struct node_t;

    typedef TriePool<node_t> pool_t;
    typedef node_attrs_t<traits_t> attrs_t;
    typedef std::forward_list<value_t> vals_t;
    typedef typename attrs_t::path_t path_t;
    typedef Expr::expr_def_t expr_def_t;

    using traits_t::ult;

    // ISO/IEC 14882:2011 23.3.4.5 forward_list
    // modifiers [forwardlist.modifiers]

    // stev: the only modifying operation which
    // is applied to 'vals' list is 'push_front';
    // by this fact, we can use safely references
    // returned by 'front()' for the entire life
    // time of the list; the C++11 standard does
    // not say explicitly whether 'push_front'
    // invalidates or not references to elements
    // of the container. It only says that (see
    // 23.3.4.5 pt. 1):
    //
    //   None of the overloads of 'insert_after'
    //   shall affect the validity of iterators
    //   and references, and 'erase_after' shall
    //   invalidate only iterators and references
    //   to the erased elements.
    //
    // stev: interestingly enough is that in the
    // case of 'list', the standard does say that
    // none of '{push,pop}_{back,front}' functions
    // invalidate iterators or references -- see
    // 23.3.5.4 list modifiers, pt. 1.

    value_t* make_value(const value_t& val)
    { vals.push_front(val); return &vals.front(); }

    value_t* make_value(value_t&& val)
    { vals.push_front(std::move(val)); return &vals.front(); }

    void gen_attrs() const
    { if (root) attrs = gen_attrs(root); }

    static attrs_t gen_attrs(node_t* node);

    static void print_obj(
        std::ostream& ost, const node_t* node,
        const char* name = nullptr,
        size_t level = 0);
    static void gen_wide_trie(
        std::ostream& ost, const node_t* node,
        size_t level = 1);
    static void gen_compact_trie(
        std::ostream& ost, const node_t* node,
        path_t& path, size_t start = 0,
        size_t level = 1);
    static void gen_wide_code(
        std::ostream& ost, const node_t* node,
        const attrs_t& attrs,
        size_t level = 1);
    static void gen_compact_code(
        std::ostream& ost, const node_t* node,
        path_t& path, const attrs_t& attrs,
        size_t start = 0,
        size_t level = 1);
    static void gen_wide_expr(
        std::ostream& ost, const node_t* node,
        path_t& path, const attrs_t& attrs,
        const expr_def_t* def,
        size_t start = 0,
        size_t level = 1);
    static void gen_compact_expr(
        std::ostream& ost, const node_t* node,
        path_t& path, const attrs_t& attrs,
        const expr_def_t* def,
        size_t start = 0,
        size_t level = 1);

    void check_key(const char_t* key) const;

    node_t* lookup(const char_t* key) const;
    node_t* insert(const char_t* key);

    node_t* root = nullptr;
    pool_t  pool;
    vals_t  vals;

    mutable //!!!HACK
    attrs_t attrs;
};

template<
    typename T,
    typename C,
    template<typename> class R>
struct TernaryTrie<T, C, R>::node_t
{
    typedef TernaryTrie<T, C, R> trie_t;
    typedef trie_t::value_t value_t;
    typedef trie_t::attrs_t attrs_t;

    node_t() noexcept
    {}
    ~node_t() noexcept
    {}

    static constexpr size_t size()
    { return sizeof(node_t); }

    bool has_child() const
    { return ch || lo || hi; }

    size_t n_child() const
    { return bool(ch) + bool(lo) + bool(hi); }

    const node_t* value_child() const
    {
        SYS_ASSERT(this != NULL);
        for (auto r = this;
             r != nullptr;
             r = r->lo) {
            // invariant:
            //  r->ch == 0 =>
            //  r->lo == nullptr
            SYS_ASSERT(
                r->ch ||
                r->lo == nullptr);
            if (r->ch == 0)
                return r;
        }
        return nullptr;
    }

    // stev: note that 'ch' is
    // the discriminator attached
    // to the union below by the
    // following conditions:
    //
    //   ch != 0 => use 'eq'
    //   ch == 0 => use 'val'

    value_t* value() const
    { SYS_ASSERT(ch == 0); return val; }

    void value(value_t* _val)
    { SYS_ASSERT(ch == 0); val = _val; }

    char_t  ch;
    node_t* lo;
    node_t* hi;
    union
    {
        node_t*  eq;
        value_t* val;
    };

    // stev: node annotation attributes
    // necessary for code generation
    attrs_t attrs;
};

template<
    typename T,
    typename C,
    template<typename> class R>
void TernaryTrie<T, C, R>::check_key(
    const char_t* key) const
{
    if (*key == 0)
        empty_key_error();
}

template<
    typename T,
    typename C,
    template<typename> class R>
typename
TernaryTrie<T, C, R>::node_t*
TernaryTrie<T, C, R>::lookup(const char_t* key) const
{
    check_key(key);

    auto node = root;
    while (node) {
        if (ult(*key, node->ch))
            node = node->lo;
        else
        if (*key == node->ch) {
            if (*key ++ == 0)
                break;
            node = node->eq;
        }
        else
            node = node->hi;
    }

    return node;
}

template<
    typename T,
    typename C,
    template<typename> class R>
typename
TernaryTrie<T, C, R>::node_t*
TernaryTrie<T, C, R>::insert(const char_t* key)
{
    check_key(key);

    node_t* node;
    auto ptr = &root;
    while ((node = *ptr)) {
        if (*key == node->ch) {
            if (*key ++ == 0)
                return node;
            ptr = &(node->eq);
        }
        else
        if (ult(*key, node->ch))
            ptr = &(node->lo);
        else
            ptr = &(node->hi);
    }

    while (true) {
        node = *ptr = pool.new_node();
        node->ch = *key;
        node->lo = node->eq = node->hi = nullptr;
        if (*key ++ == 0)
            break;
        ptr = &(node->eq);
    }

    return node;
}

template<
    typename T,
    typename C,
    template<typename> class R>
void TernaryTrie<T, C, R>::print_obj(
    std::ostream& ost, const node_t* node,
    const char* name, size_t level)
{
    using namespace Ext;

    SYS_ASSERT(node != nullptr);

    if (name)
        ost << indent(level)
            << '.'
            << name
            << ' ';
    if (globals->print_attributes)
        ost << node->attrs
            << ' ';
    if (true)
        ost << "{\n"
            << indent(level + 1);
    if (node->ch)
        ost << ".ch: "
            << repr(node->ch);
    else
        ost << ".val: "
            << repr(*node->value());
    if (true)
        ost << '\n';

    if (node->lo)
        print_obj(ost, node->lo, "lo", level + 1);

    if (node->ch && node->eq)
        print_obj(ost, node->eq, "eq", level + 1);

    if (node->hi)
        print_obj(ost, node->hi, "hi", level + 1);

    if (true)
        ost << indent(level)
            << "}\n";
}

template<
    typename T,
    typename C,
    template<typename> class R>
typename
TernaryTrie<T, C, R>::attrs_t
TernaryTrie<T, C, R>::gen_attrs(
    node_t* node)
{
    SYS_ASSERT(node != nullptr);

    attrs_t a;

    if (node->lo)
        a += gen_attrs(node->lo);

    if (node->ch && node->eq) {
        node->attrs = gen_attrs(node->eq);

        a += attrs_t{1, node->ch};
    }

    if (node->hi)
        a += gen_attrs(node->hi);

    return a;
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline void TernaryTrie<T, C, R>::print(
    std::ostream& ost) const
{
    // stev: generate needed
    // annotation attributes
    gen_attrs();

    // stev: root == nullptr does have
    // meaning: the trie is empty
    if (root) {
        if (globals->print_attributes)
            ost << attrs
                << '\n';
        print_obj(ost, root);
    }
}

template<
    typename T,
    typename C,
    template<typename> class R>
void TernaryTrie<T, C, R>::gen_wide_trie(
    std::ostream& ost, const node_t* node,
    size_t level)
{
    using namespace Ext;

    SYS_ASSERT(node != nullptr);

    if (node->lo)
        gen_wide_trie(ost, node->lo, level);

    if (node->ch && node->eq) {
        if (true)
            ost << indent(level)
                << repr(node->ch);

        if (const node_t* child =
                node->eq->value_child())
            ost << ": "
                << repr(*child->value());

        if (globals->print_attributes &&
            node->attrs)
            ost << ' '
                << node->attrs;

        auto at = node->eq->has_child();
        if (at)
            ost << " {";
        if (true)
            ost << '\n';

        gen_wide_trie(ost, node->eq, level + 1);

        if (at)
            ost << indent(level)
                << "}\n";
    }

    if (node->hi)
        gen_wide_trie(ost, node->hi, level);
}

template<
    typename T,
    typename C,
    template<typename> class R>
void TernaryTrie<T, C, R>::gen_compact_trie(
    std::ostream& ost, const node_t* node,
    path_t& path, size_t start, size_t level)
{
    using namespace Ext;

    SYS_ASSERT(node != nullptr);

    if (node->lo)
        gen_compact_trie(
            ost, node->lo, path, start, level);

    if (node->ch && node->eq) {
        const auto n =
            node->eq->n_child();
        const auto child =
            node->eq->value_child();
        const auto at =
            !child && n == 1;
        const auto obj =
            n && !at;

        const auto pd = path.data();
        const auto ps = path.size();
        SYS_ASSERT(ps >= start);

        if (at)
            path.push_back(node->ch);

        // stev: !at <=> child || n != 1;
        //       therefore: child => !at
        if (!at)
            ost << indent(level)
                << '"'
                << repr(pd + start, pd + ps, false)
                << repr(node->ch, false)
                << '"';

        if (!at && globals->print_attributes)
            ost << ' '
                << print_attrs(
                    node->attrs, node->ch,
                    path, start);

        // stev: child => !at (see above)
        if (child)
            ost << ": "
                << repr(*child->value());

        // stev: obj <=> n && child || n == 1
        if (obj)
            ost << " {";

        // stev: obj || child <=> child || n == 1
        if (obj || child)
            ost << '\n';

        gen_compact_trie(
            ost, node->eq, path,
            !at ? ps : start,
            level + obj);

        if (obj)
            ost << indent(level)
                << "}\n";

        if (at)
            path.pop_back();
    }

    if (node->hi)
        gen_compact_trie(
            ost, node->hi, path, start, level);
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline void TernaryTrie<T, C, R>::gen_trie(
    std::ostream& ost,
    gen_type_t type)
    const
{
    // stev: generate needed
    // annotation attributes
    gen_attrs();

    // stev: root == nullptr does have
    // meaning: the trie is empty
    if (!root)
        return;

    using namespace Ext;

    if (globals->print_attributes)
        ost << attrs
            << ' ';

    ost << "{\n";

    if (type == gen_type_t::compact) {
        path_t path;
        gen_compact_trie(ost, root, path);
    }
    else
    if (type == gen_type_t::wide)
        gen_wide_trie(ost, root);
    else
        SYS_UNEXPECT_ERR(
            "unknown gen-type: %zu",
            Num::integer_cast<size_t>(
                type));

    ost << "}\n";
}

template<
    typename T,
    typename C,
    template<typename> class R>
void TernaryTrie<T, C, R>::gen_wide_code(
    std::ostream& ost, const node_t* node,
    const attrs_t& attrs,
    size_t level)
{
    using namespace Ext;

    SYS_ASSERT(node != nullptr);

    if (node->lo)
        gen_wide_code(
            ost, node->lo, attrs, level);

    if (node->ch && node->eq) {
        if (globals->print_attributes)
            ost << indent(level)
                << "// ";
        if (globals->print_attributes)
            ost << repr(node->ch)
                << ' ';
        if (globals->print_attributes)
            ost << node->attrs
                << '\n';

        if (true)
            ost << indent(level)
                << cond(attrs.width > 1, "case", "if (*p ++ ==")
                << ' '
                << repr(node->ch)
                << cond(attrs.width > 1, ':', ") {")
                << '\n';

        if (const auto child = node->eq->value_child())
            ost << indent(level + 1)
                << "if (*p == 0)\n"
                << indent(level + 2)
                << "return "
                << repr(*child->value())
                << ";\n";

        if (node->attrs.width > 1)
            ost << indent(level + 1)
                << "switch (*p ++) {\n";

        gen_wide_code(
            ost, node->eq, node->attrs, level + 1);

        if (node->attrs.width > 1)
            ost << indent(level + 1)
                << "}\n";

        if (attrs.width <= 1)
            ost << indent(level)
                << "}\n";

        if (attrs.last != node->ch)
            ost << indent(level + 1)
                << "return error;\n";
    }

    if (node->hi)
        gen_wide_code(
            ost, node->hi, attrs, level);
}

template<
    typename T,
    typename C,
    template<typename> class R>
void TernaryTrie<T, C, R>::gen_compact_code(
    std::ostream& ost, const node_t* node,
    path_t& path, const attrs_t& attrs,
    size_t start, size_t level)
{
    using namespace Ext;

    SYS_ASSERT(node != nullptr);

    if (node->lo)
        gen_compact_code(
            ost, node->lo, path,
            attrs, start, level);

    if (node->ch && node->eq) {
        const auto n =
            node->eq->n_child();
        const auto child =
            node->eq->value_child();
        const auto at =
            !child && n == 1;
        const auto obj =
            n && !at;

        const auto pd = path.data();
        const auto ps = path.size();
        SYS_ASSERT(ps >= start);
        const size_t l = ps - start;

#if defined(DEBUG) && \
    defined(DEBUG_TRIE_TERNARY_COMPACT_CODE)
        const auto dump_vars = [&](std::ostream& ost) {
            ost << indent(level)
                << "//"
                << " path=" << repr(path)
                << " attrs=" << attrs << ',' << node->attrs
                << " start=" << start
                << " ch=" << repr(node->ch)
                << " n=" << n
                << " child=" << bool(child)
                << " at=" << at
                << " obj=" << obj
                << " ps=" << ps
                << " l=" << l
                << '\n';
        };
        if (globals->debug > 1)
            ost << dump_vars;
#endif

        if (at)
            path.push_back(node->ch);

        if (!at && globals->print_attributes)
            ost << indent(level)
                << "// "
                << print_attrs(
                    node->attrs, node->ch,
                    path, start)
                << '\n';

        const auto b = l > 0;
        const auto c =
            !child || node->attrs.width;
        const auto w = attrs.width > 1;
        const auto s = start + (w && b);
        const auto t = !w || b;
        const auto i = t || s < ps;
        const auto j = c && i;
        const auto d = 1 + (w && b);
        const auto a = c || !i;

#if defined(DEBUG) && \
    defined(DEBUG_TRIE_TERNARY_COMPACT_CODE)
        const auto dump_vars2 = [&](std::ostream& ost) {
            ost << indent(level)
                << "//"
                << " attrs=" << attrs << ',' << node->attrs
                << " b=" << b
                << " c=" << c
                << " w=" << w
                << " s=" << s
                << " t=" << t
                << " i=" << i
                << " j=" << j
                << " d=" << d
                << " a=" << a
                << '\n';
        };
#endif
        // stev: !at <=> child || n != 1;
        //       therefore: child => !at
        if (!at) {
            if (w)
                ost << indent(level)
                    << "case "
                    << repr(b ? pd[start] : node->ch)
                    << ":\n";
            if (i)
                ost << indent(level + w)
                    << "if (";
            for (auto p = pd + s, e = pd + ps;
                p < e; ++ p) {
                ost << "*p ++ == "
                    << repr(*p);
                if (p + 1 < e || t)
                    ost << " &&\n"
                        << indent(level + w + 1);
            }
            if (t)
                ost << "*p ++ == "
                    << repr(node->ch);
            if (j)
                ost << ") {\n";
        }

        // stev: child => !at (see above)
        if (child) {
#if defined(DEBUG) && \
    defined(DEBUG_TRIE_TERNARY_COMPACT_CODE)
            if (globals->debug > 1)
                ost << cond(!a, '\n')
                    << dump_vars
                    << dump_vars2;
#endif
            if (a)
                ost << indent(level + d)
                    << "if (";
            else
                ost << " &&\n"
                    << indent(level + d);

            ost << "*p == 0)\n"
                << indent(level + d + a)
                << "return "
                << repr(*child->value())
                << ";\n";
        }

        if (obj && node->attrs.width > 1)
            ost << indent(level + d)
                << "switch (*p ++) {\n";

        gen_compact_code(
            ost, node->eq, path,
            obj ? node->attrs : attrs,
            !at ? ps : start,
            level + (obj ? d : 0));

        if ((obj && node->attrs.width > 1) ||
            DEBUG_CONDITION(8)) {
#if defined(DEBUG) && \
    defined(DEBUG_TRIE_TERNARY_COMPACT_CODE)
            if (globals->debug > 1)
                ost << dump_vars
                    << dump_vars2;
#endif
            ost << indent(level + d)
                << "}\n";
        }

        if ((!at && j) ||
            DEBUG_CONDITION(8)) {
#if defined(DEBUG) && \
    defined(DEBUG_TRIE_TERNARY_COMPACT_CODE)
            if (globals->debug > 1)
                ost << dump_vars
                    << dump_vars2;
#endif
            ost << indent(level + w)
                << "}\n";
        }

        if ((attrs.last != node->ch && !l) ||
            DEBUG_CONDITION(9)) {
#if defined(DEBUG) && \
    defined(DEBUG_TRIE_TERNARY_COMPACT_CODE)
            if (globals->debug > 1)
                ost << dump_vars;
#endif
            ost << indent(level + 1)
                << "return error;\n";
        }

        if (at)
            path.pop_back();
    }

    if (node->hi)
        gen_compact_code(
            ost, node->hi, path,
            attrs, start, level);
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline void TernaryTrie<T, C, R>::gen_code(
    std::ostream& ost,
    gen_type_t type)
    const
{
    // stev: generate needed
    // annotation attributes
    gen_attrs();

    using namespace Ext;

    if (globals->print_attributes)
        ost << indent(1)
            << "// "
            << attrs
            << '\n';

    // stev: TODO: refactor 'gen_{,wide_}code'
    // such that to eliminate the duplication
    // of code that prints 'switch (*p ++) {\n'

    if (attrs.width > 1)
        ost << indent(1)
            << "switch (*p ++) {\n";

    // stev: root == nullptr does have
    // meaning: the trie is empty
    if (root) {
        if (type == gen_type_t::compact) {
            path_t path;
            gen_compact_code(
                ost, root, path, attrs);
        }
        else
        if (type == gen_type_t::wide)
            gen_wide_code(
                ost, root, attrs);
        else
            SYS_UNEXPECT_ERR(
                "unknown gen-type: %zu",
                Num::integer_cast<size_t>(
                    type));
    }

    if (attrs.width > 1)
        ost << indent(1)
            << "}\n";

    if (true)
        ost << indent(1)
            << "return error;\n";
}

template<
    typename T,
    typename C,
    template<typename> class R>
void TernaryTrie<T, C, R>::gen_wide_expr(
    std::ostream& ost, const node_t* node,
    path_t& path, const attrs_t& attrs,
    const expr_def_t* def,
    size_t start,
    size_t level)
{
    using namespace Ext;

    SYS_ASSERT(node != nullptr);

    SYS_ASSERT(
        globals->expr_depth == 0 ||
        globals->expr_depth >= level);

    if (node->lo)
        gen_wide_expr(
            ost, node->lo, path, attrs,
            def, start, level);

    if (node->ch && node->eq) {
        const auto n =
            node->eq->n_child();
        const auto child =
            node->eq->value_child();
        const auto at =
            !child && n == 1;
        const auto obj =
            n && !at;
        const auto alt =
            attrs.width > 1 &&
            attrs.last != node->ch;

        const auto pd = path.data();
        const auto ps = path.size();
        SYS_ASSERT(ps >= start);

#if defined(DEBUG) && \
    defined(DEBUG_TRIE_TERNARY_WIDE_EXPR)
        const auto dump_vars = [&](std::ostream& ost) {
            ost << '\n'
                << indent(level)
                << "//"
                << " level=" << level
                << " path=" << repr(path)
                << " attrs=" << attrs << ',' << node->attrs
                << " start=" << start
                << " ch=" << repr(node->ch)
                << " n=" << n
                << " child=" << bool(child)
                << " at=" << at
                << " obj=" << obj
                << " ps=" << ps
                << " alt=" << alt;
        };
        if (globals->debug > 1)
            dump_vars(ost);
#endif

        if (at)
            path.push_back(node->ch);

        // stev: !at <=> child || n != 1;
        //       therefore: child => !at
        if (!at) {
            using namespace Expr;
            ost << '\n'
                << indent(level)
                << repr(pd + start, pd + ps, def, true)
                << repr(node->ch, def, true);
        }

        if (!at && globals->print_attributes)
            ost << ' '
                << print_attrs(
                    node->attrs, node->ch,
                    path, start);

        if (globals->expr_depth == 0 ||
            globals->expr_depth >= level + obj) {
            // stev: obj <=> n && child || n == 1
            if (obj)
                ost << ' '
                    << (child
                        ? def->open_opt_paren
                        : def->open_paren);

            gen_wide_expr(
                ost, node->eq, path,
                node->attrs, def,
                !at ? ps : start,
                level + obj);

            if (obj)
                ost << '\n'
                    << indent(level)
                    << (child
                        ? def->close_opt_paren
                        : def->close_paren);
        }

        if (alt)
            ost << cond(def->sep, ' ')
                << def->alter;

        if (at)
            path.pop_back();
    }

    if (node->hi)
        gen_wide_expr(
            ost, node->hi, path, attrs,
            def, start, level);
}

template<
    typename T,
    typename C,
    template<typename> class R>
void TernaryTrie<T, C, R>::gen_compact_expr(
    std::ostream& ost, const node_t* node,
    path_t& path, const attrs_t& attrs,
    const expr_def_t* def,
    size_t start,
    size_t level)
{
    using namespace Ext;

    SYS_ASSERT(node != nullptr);

    SYS_ASSERT(
        globals->expr_depth == 0 ||
        globals->expr_depth >= level);

    if (node->lo)
        gen_compact_expr(
            ost, node->lo, path, attrs,
            def, start, level);

    if (node->ch && node->eq) {
        const auto n =
            node->eq->n_child();
        const auto child =
            node->eq->value_child();
        const auto at =
            !child && n == 1;
        const auto obj =
            n && !at;
        const auto alt =
            attrs.width > 1 &&
            attrs.last != node->ch;

        const auto pd = path.data();
        const auto ps = path.size();
        SYS_ASSERT(ps >= start);

#if defined(DEBUG) && \
    defined(DEBUG_TRIE_TERNARY_COMPACT_EXPR)
        const auto dump_vars = [&](std::ostream& ost) {
            ost << "//"
                << " level=" << level
                << " path=" << repr(path)
                << " attrs=" << attrs << ',' << node->attrs
                << " start=" << start
                << " ch=" << repr(node->ch)
                << " n=" << n
                << " child=" << bool(child)
                << " at=" << at
                << " obj=" << obj
                << " ps=" << ps
                << " alt=" << alt
                << '\n';
        };
        if (globals->debug > 1)
            dump_vars(ost);
#endif

        if (at)
            path.push_back(node->ch);

        // stev: !at <=> child || n != 1;
        //       therefore: child => !at
        if (!at) {
            using namespace Expr;
            ost << repr(pd + start, pd + ps, def, false)
                << repr(node->ch, def, false);
        }

        if (!at && globals->print_attributes)
            ost << ' '
                << print_attrs(
                    node->attrs, node->ch,
                    path, start);

        if (globals->expr_depth == 0 ||
            globals->expr_depth >= level + obj) {
            // stev: obj <=> n && child || n == 1
            if (obj)
                ost << (child
                    ? def->open_opt_paren
                    : def->open_paren);

            gen_compact_expr(
                ost, node->eq, path,
                node->attrs, def,
                !at ? ps : start,
                level + obj);

            if (obj)
                ost << (child
                    ? def->close_opt_paren
                    : def->close_paren);
        }

        if (alt)
            ost << def->alter;

        if (at)
            path.pop_back();
    }

    if (node->hi)
        gen_compact_expr(
            ost, node->hi, path, attrs,
            def, start, level);
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline void TernaryTrie<T, C, R>::gen_expr(
    std::ostream& ost,
    gen_type_t type)
    const
{
    // stev: generate needed
    // annotation attributes
    gen_attrs();

    // stev: root == nullptr does have
    // meaning: the trie is empty
    if (!root)
        return;

    const auto wide = type == gen_type_t::wide;
    const auto bash = globals->expr_type ==
        Expr::expr_type_t::bash;

    if (wide) {
        if (globals->print_attributes)
            ost << attrs
                << ' ';
    }
    else
    if (type != gen_type_t::compact)
        SYS_UNEXPECT_ERR(
            "unknown gen-type: %zu",
            Num::integer_cast<size_t>(
                type));

    path_t path;
    auto def = expr_def_t::get_def(globals->expr_type);

    using namespace Ext;
    ost << cond(wide || bash, def->open_paren);

    if (wide)
        gen_wide_expr(
            ost, root, path, attrs, def);
    else
        gen_compact_expr(
            ost, root, path, attrs, def);

    ost << cond(wide, '\n')
        << cond(wide || bash, def->close_paren)
        << '\n';
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline Ext::ostream_manip_t print(
    const TernaryTrie<T, C, R>& trie)
{
    typedef
        TernaryTrie<T, C, R> trie_t;
    return
        Ext::manip(&trie_t::print, &trie);
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline Ext::ostream_manip_t gen_trie(
    const TernaryTrie<T, C, R>& trie, gen_type_t type)
{
    typedef
        TernaryTrie<T, C, R> trie_t;
    return
        Ext::manip(&trie_t::gen_trie, &trie, type);
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline Ext::ostream_manip_t gen_code(
    const TernaryTrie<T, C, R>& trie, gen_type_t type)
{
    typedef
        TernaryTrie<T, C, R> trie_t;
    return
        Ext::manip(&trie_t::gen_code, &trie, type);
}

template<
    typename T,
    typename C,
    template<typename> class R>
inline Ext::ostream_manip_t gen_expr(
    const TernaryTrie<T, C, R>& trie, gen_type_t type)
{
    typedef
        TernaryTrie<T, C, R> trie_t;
    return
        Ext::manip(&trie_t::gen_expr, &trie, type);
}

} // namespace Trie

#endif /* __TRIE_HPP */


