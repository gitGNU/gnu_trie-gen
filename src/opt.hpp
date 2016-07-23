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

#ifndef __OPT_HPP
#define __OPT_HPP

#include "sys.hpp"
#include "num.hpp"
#include "glob.hpp"

namespace Opt {

struct Error : public Sys::Error
{
    explicit Error(const char* what) :
        Sys::Error{what}
    {}
};

class options_t :
    private global_options_t
{
public:
    typedef size_t argc_t;
    typedef char* const* argv_t;

    enum class node_type_t {
        integer,
        floating_point,
        string
    };
    enum class trie_type_t {
        array,
        ternary
    };
    enum class output_type_t {
        object,
        c_code,
        trie,
        expr,
    };
    enum class gen_type_t {
        compact,
        wide,
    };

    node_type_t   node_type   = node_type_t::string;
    trie_type_t   trie_type   = trie_type_t::ternary;
    output_type_t output_type = output_type_t::object;
    gen_type_t    gen_type    = gen_type_t::wide;

    argc_t argc = 0;
    argv_t argv = nullptr;

private:
    friend
    const options_t* parse_options(
        Num::box_t<argc_t> argc, argv_t argv);

    void parse(argc_t argc, argv_t argv);

    static void error(const char* msg, ...) PRINTF_FMT(1);

    static void invalid_opt_arg(const char* opt_name, const char* opt_arg);
    static void missing_opt_arg(const char* opt_name);
    static void missing_opt_arg(char opt_name);
    static void not_allowed_opt_arg(const char* opt_name);
    static void invalid_opt(const char* opt_name);
    static void invalid_opt(char opt_name);

#ifdef DEBUG
    typedef unsigned short debug_t;
    static debug_t parse_debug_optarg(const char* opt_arg);
#endif

    static bool lookup_node(const char*, node_type_t&);
    static node_type_t parse_node_type_optarg(const char* opt_arg);

    static bool lookup_trie(const char*, trie_type_t&);
    static trie_type_t parse_trie_type_optarg(const char* opt_arg);

    static bool lookup_output(const char*, output_type_t&);
    static output_type_t parse_output_type_optarg(const char* opt_arg);

    static bool lookup_gen(const char*, gen_type_t&);
    static gen_type_t parse_gen_type_optarg(const char* opt_arg);

    static bool lookup_expr(const char*, expr_type_t&);
    static expr_type_t parse_expr_type_optarg(const char* opt_arg);

    typedef decltype(global_options_t::expr_depth) expr_depth_t;

    static expr_depth_t parse_expr_depth_optarg(const char* opt_arg)
    { return parse_num_optarg<expr_depth_t>("expr-depth", opt_arg); }

    static size_t parse_args_optarg(
        const char *opt_name, const char *opt_arg,
        char const* const* args, size_t n_args);

    template<typename T>
    static typename std::enable_if<Num::is_integer<T>(), T>::type
        parse_args_optarg(
            const char* opt_name, const char* opt_arg,
            const char* const* args, size_t n_args)
    {
        return Num::integer_cast<T>(
            parse_args_optarg(opt_name, opt_arg, args, n_args));
    }

    template<typename T>
    static typename std::enable_if<Num::is_enum<T>(), T>::type
        parse_args_optarg(
            const char* opt_name, const char* opt_arg,
            const char* const* args, size_t n_args)
    {
        return Num::enum_cast<T>(
            parse_args_optarg(opt_name, opt_arg, args, n_args));
    }

    template<typename T>
    static typename std::enable_if<Num::is_integer<T>(), T>::type
        parse_num_optarg(
            const char* opt_name, const char* opt_arg)
    {
        T r = 0;
        if (!Num::parse_num(opt_arg, r))
            invalid_opt_arg(opt_name, opt_arg);
        return r;
    }
};

// stev: we are boxing the 'argc' values comming from outside.
const options_t* parse_options(
    Num::box_t<options_t::argc_t> argc, options_t::argv_t argv);

} // namespace Opt

#endif /* __OPT_HPP */


