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

#include "config.h"

#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <cctype>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <getopt.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>

#include "sys.hpp"
#include "num.hpp"
#include "ext.hpp"
#include "opt.hpp"

namespace Opt {

const options_t* parse_options(
    Num::box_t<options_t::argc_t> argc,
    options_t::argv_t argv)
{
    static options_t opts;
    opts.parse(argc, argv);
    return &opts;
}

void options_t::error(const char* msg, ...)
{
    STATIC_ASSERT(
        CONFIG_VA_END_NOOP);
    va_list args;
    va_start(args, msg);
    Sys::verror<Error>(msg, args);
    //!!!VA_END va_end(args);
}

void options_t::invalid_opt_arg(const char* opt_name, const char* opt_arg)
{
    error("invalid argument for '%s' option: '%s'", opt_name, opt_arg);
}

void options_t::missing_opt_arg(const char* opt_name)
{
    error("argument for option '%s' not found", opt_name);
}

void options_t::missing_opt_arg(char opt_name)
{
    error("argument for option '-%c' not found", opt_name);
}

void options_t::not_allowed_opt_arg(const char* opt_name)
{
    error("option '%s' does not allow an argument", opt_name);
}

void options_t::invalid_opt(const char* opt_name)
{
    error("invalid command line option '%s'", opt_name);
}

void options_t::invalid_opt(char opt_name)
{
    error("invalid command line option '-%c'", opt_name);
}

#ifdef DEBUG
options_t::debug_t options_t::parse_debug_optarg(
    const char* opt_arg)
{
    if (opt_arg == nullptr)
        return 1;
    else {
        if (!isdigit(opt_arg[0]) || opt_arg[1] != '\0')
            invalid_opt_arg("debug", opt_arg);
        return
            Num::integer_cast<debug_t>(*opt_arg - '0');
    }
}
#endif

// $ print() { printf '%s\n' "$@"; }
// $ print 'i int =integer' 'f float =floating_point' 's str string'|gen-func -f options_t::lookup_node -r node_type_t -v

bool options_t::lookup_node(
    const char* n, node_type_t& t)
{
    // stev: pattern: i[nt]|f[loat]|s[tr[ing]]
    switch (*n ++) {
    case 'f':
        if (*n == 0) {
            t = node_type_t::floating_point;
            return true;
        }
        if (*n ++ == 'l' &&
            *n ++ == 'o' &&
            *n ++ == 'a' &&
            *n ++ == 't' &&
            *n == 0) {
            t = node_type_t::floating_point;
            return true;
        }
        return false;
    case 'i':
        if (*n == 0) {
            t = node_type_t::integer;
            return true;
        }
        if (*n ++ == 'n' &&
            *n ++ == 't' &&
            *n == 0) {
            t = node_type_t::integer;
            return true;
        }
        return false;
    case 's':
        if (*n == 0) {
            t = node_type_t::string;
            return true;
        }
        if (*n ++ == 't'  &&
            *n ++ == 'r') {
            if (*n == 0) {
                t = node_type_t::string;
                return true;
            }
            if (*n ++ == 'i' &&
                *n ++ == 'n' &&
                *n ++ == 'g' &&
                *n == 0) {
                t = node_type_t::string;
                return true;
            }
        }
    }
    return false;
}

options_t::node_type_t options_t::parse_node_type_optarg(
    const char *opt_arg)
{
    node_type_t type = node_type_t::string;
    if (opt_arg && !lookup_node(opt_arg, type))
        invalid_opt_arg("node-type", opt_arg);
    return type;
}

// $ print 'a array' 't ternary'|gen-func -f options_t::lookup_trie -r trie_type_t -v

bool options_t::lookup_trie(const char* n, trie_type_t& t)
{
    // stev: pattern: a[rray]|t[ernary]
    switch (*n ++) {
    case 'a':
        if (*n == 0) {
            t = trie_type_t::array;
            return true;
        }
        if (*n ++ == 'r' &&
            *n ++ == 'r' &&
            *n ++ == 'a' &&
            *n ++ == 'y' &&
            *n == 0) {
            t = trie_type_t::array;
            return true;
        }
        return false;
    case 't':
        if (*n == 0) {
            t = trie_type_t::ternary;
            return true;
        }
        if (*n ++ == 'e' &&
            *n ++ == 'r' &&
            *n ++ == 'n' &&
            *n ++ == 'a' &&
            *n ++ == 'r' &&
            *n ++ == 'y' &&
            *n == 0) {
            t = trie_type_t::ternary;
            return true;
        }
    }
    return false;
}

options_t::trie_type_t options_t::parse_trie_type_optarg(
    const char *opt_arg)
{
    trie_type_t type = trie_type_t::array;
    if (opt_arg && !lookup_trie(opt_arg, type))
        invalid_opt_arg("trie-type", opt_arg);
    return type;
}

// $ print 'o obj object' 'c c-code =c_code' 't trie' 'e expr'|gen-func -f options_t::lookup_output -r output_type_t -v

bool options_t::lookup_output(const char* n, output_type_t& t)
{
    // stev: pattern: o[bj[ject]|c[-code]|t[rie]|e[xpr]
    switch (*n ++) {
    case 'c':
        if (*n == 0) {
            t = output_type_t::c_code;
            return true;
        }
        if (*n ++ == '-' &&
            *n ++ == 'c' &&
            *n ++ == 'o' &&
            *n ++ == 'd' &&
            *n ++ == 'e' &&
            *n == 0) {
            t = output_type_t::c_code;
            return true;
        }
        return false;
    case 'e':
        if (*n == 0) {
            t = output_type_t::expr;
            return true;
        }
        if (*n ++ == 'x' &&
            *n ++ == 'p' &&
            *n ++ == 'r' &&
            *n == 0) {
            t = output_type_t::expr;
            return true;
        }
        return false;
    case 'o':
        if (*n == 0) {
            t = output_type_t::object;
            return true;
        }
        if (*n ++ == 'b' &&
            *n ++ == 'j') {
            if (*n == 0) {
                t = output_type_t::object;
                return true;
            }
            if (*n ++ == 'e' &&
                *n ++ == 'c' &&
                *n ++ == 't' &&
                *n == 0) {
                t = output_type_t::object;
                return true;
            }
        }
        return false;
    case 't':
        if (*n == 0) {
            t = output_type_t::trie;
            return true;
        }
        if (*n ++ == 'r' &&
            *n ++ == 'i' &&
            *n ++ == 'e' &&
            *n == 0) {
            t = output_type_t::trie;
            return true;
        }
    }
    return false;
}

options_t::output_type_t options_t::parse_output_type_optarg(
    const char *opt_arg)
{
    output_type_t type = output_type_t::object;
    if (opt_arg && !lookup_output(opt_arg, type))
        invalid_opt_arg("output-type", opt_arg);
    return type;
}

// $ print 'c comp compact' 'w wide'|gen-func -f options_t::lookup_gen -r gen_type_t -v

bool options_t::lookup_gen(
    const char* n, gen_type_t& t)
{
    // stev: pattern: c[omp[act]]|w[ide]
    switch (*n ++) {
    case 'c':
        if (*n == 0) {
            t = gen_type_t::compact;
            return true;
        }
        if (*n ++ == 'o' &&
            *n ++ == 'm' &&
            *n ++ == 'p') {
            if (*n == 0) {
                t = gen_type_t::compact;
                return true;
            }
            if (*n ++ == 'a' &&
                *n ++ == 'c' &&
                *n ++ == 't' &&
                *n == 0) {
                t = gen_type_t::compact;
                return true;
            }
        }
        return false;
    case 'w':
        if (*n == 0) {
            t = gen_type_t::wide;
            return true;
        }
        if (*n ++ == 'i' &&
            *n ++ == 'd' &&
            *n ++ == 'e' &&
            *n == 0) {
            t = gen_type_t::wide;
            return true;
        }
    }
    return false;
}

options_t::gen_type_t options_t::parse_gen_type_optarg(
    const char *opt_arg)
{
    gen_type_t type = gen_type_t::compact;
    if (opt_arg && !lookup_gen(opt_arg, type))
        invalid_opt_arg("gen-type", opt_arg);
    return type;
}

// $ print 'c cxxpy' 'p perl' 'b bash'|gen-func -f options_t::lookup_expr -r expr_type_t -v

bool options_t::lookup_expr(
    const char* n, expr_type_t& t)
{
    // stev: pattern: c[xxpy]|p[erl]|b[ash]
    switch (*n ++) {
    case 'b':
        if (*n == 0) {
            t = expr_type_t::bash;
            return true;
        }
        if (*n ++ == 'a' &&
            *n ++ == 's' &&
            *n ++ == 'h' &&
            *n == 0) {
            t = expr_type_t::bash;
            return true;
        }
        return false;
    case 'c':
        if (*n == 0) {
            t = expr_type_t::cxxpy;
            return true;
        }
        if (*n ++ == 'x' &&
            *n ++ == 'x' &&
            *n ++ == 'p' &&
            *n ++ == 'y' &&
            *n == 0) {
            t = expr_type_t::cxxpy;
            return true;
        }
        return false;
    case 'p':
        if (*n == 0) {
            t = expr_type_t::perl;
            return true;
        }
        if (*n ++ == 'e' &&
            *n ++ == 'r' &&
            *n ++ == 'l' &&
            *n == 0) {
            t = expr_type_t::perl;
            return true;
        }
    }
    return false;
}

options_t::expr_type_t options_t::parse_expr_type_optarg(
    const char *opt_arg)
{
    expr_type_t type = expr_type_t::cxxpy;
    if (opt_arg && !lookup_expr(opt_arg, type))
        invalid_opt_arg("expr-type", opt_arg);
    return type;
}

size_t options_t::parse_args_optarg(
    const char *opt_name, const char *opt_arg,
    char const* const* args, size_t n_args)
{
    char const* const* end = args + n_args;
    char const* const *ptr = std::find_if(
        args, end, [=](const char* arg) {
            return strcmp(arg, opt_arg) == 0;
        });
    if (ptr >= end)
        invalid_opt_arg(opt_name, opt_arg);
    return
        Num::integer_cast<size_t>(ptr - args);
}

void options_t::parse(argc_t argc, argv_t argv)
{
    typedef int opt_t;
    struct opt_type_t
    {
        // stev: not using an 'enum struct' since
        // we want direct equivalence between the
        // constants defines below and 'int' --
        // i.e avoid the use of static_cast, which
        // otherwise would impose itself
        enum type_t : int {
            node_type        = 't',
            trie_type        = 'T',
            output_type      = 'o',
            gen_type         = 'g',
            escape_chars     = 'c',
            expr_type        = 'e',
            expr_depth       = 'D',
            print_attributes = 'a',
            print_statistics = 's',
            print_dots       = 'd',
            verbose          = 'V',
            version          = 'v',

            no_print_attributes
                             = Num::max<char>() + 1,
            no_print_statistics,
            no_print_dots,
            no_verbose,
#ifdef DEBUG
            debug,
            no_debug,
#endif
            dump_options,
            help
        };
    };
    struct bits_opts_t {
        unsigned dump: 1;
        unsigned usage: 1;
        unsigned version: 1;
    };
    struct bits_opts_t bits = {
        .dump    = false,
        .usage   = false,
        .version = false,
    };

    static const char shorts[] =
        ":ac:dD:e:g:o:st:T:vV";
    static const struct option longs[] = {
        { "node-type",           1, nullptr, opt_type_t::node_type },
        { "trie-type",           1, nullptr, opt_type_t::trie_type },
        { "output-type",         1, nullptr, opt_type_t::output_type },
        { "gen-type",            1, nullptr, opt_type_t::gen_type },
        { "escape-chars",        1, nullptr, opt_type_t::escape_chars },
        { "expr",                1, nullptr, opt_type_t::expr_type },
        { "expr-type",           1, nullptr, opt_type_t::expr_type },
        { "expr-depth",          1, nullptr, opt_type_t::expr_depth },
        { "depth",               1, nullptr, opt_type_t::expr_depth },
        { "print-attributes",    0, nullptr, opt_type_t::print_attributes },
        { "attributes",          0, nullptr, opt_type_t::print_attributes },
        { "attrs",               0, nullptr, opt_type_t::print_attributes },
        { "no-print-attributes", 0, nullptr, opt_type_t::no_print_attributes },
        { "no-attributes",       0, nullptr, opt_type_t::no_print_attributes },
        { "no-attrs",            0, nullptr, opt_type_t::no_print_attributes },
        { "print-statistics",    0, nullptr, opt_type_t::print_statistics },
        { "statistics",          0, nullptr, opt_type_t::print_statistics },
        { "stats",               0, nullptr, opt_type_t::print_statistics },
        { "no-print-statistics", 0, nullptr, opt_type_t::no_print_statistics },
        { "no-statistics",       0, nullptr, opt_type_t::no_print_statistics },
        { "no-stats",            0, nullptr, opt_type_t::no_print_statistics },
        { "print-dots",          0, nullptr, opt_type_t::print_dots },
        { "dots",                0, nullptr, opt_type_t::print_dots },
        { "no-print-dots",       0, nullptr, opt_type_t::no_print_dots },
        { "no-dots",             0, nullptr, opt_type_t::no_print_dots },
#ifdef DEBUG
        { "debug",               2, nullptr, opt_type_t::debug },
        { "no-debug",            0, nullptr, opt_type_t::no_debug },
#endif
        { "dump-options",        0, nullptr, opt_type_t::dump_options },
        { "dump-opts",           0, nullptr, opt_type_t::dump_options },
        { "verbose",             0, nullptr, opt_type_t::verbose },
        { "no-verbose",          0, nullptr, opt_type_t::no_verbose },
        { "version",             0, nullptr, opt_type_t::version },
        { "help",                0, &optopt, opt_type_t::help },
        { nullptr,               0, nullptr, 0 }
    };
    static const char* node_types[] = {
        "int",     // node_type_t::integer
        "float",   // node_type_t::floating_point
        "string",  // node_type_t::string
    };
    static const char* trie_types[] = {
        "array",   // trie_type_t::array
        "ternary", // trie_type_t::ternary
    };
    static const char* output_types[] = {
        "object",  // output_type_t::object
        "c-code",  // output_type_t::c_code
        "trie",    // output_type_t::trie
        "expr",    // output_type_t::expr
    };
    static const char* gen_types[] = {
        "compact", // gen_type_t::compact
        "wide",    // gen_type_t::wide
    };
    static const char* expr_types[] = {
        "cxxpy",   // expr_type_t::cxxpy
        "perl",    // expr_type_t::perl
        "bash",    // expr_type_t::bash
    };

    auto argv_optind = [=]() -> char* {
        SYS_ASSERT(
            optind > 0 &&
            Num::integer_cast<size_t>(optind) - 1 <
            argc);
        return argv[optind - 1];
    };
    auto optopt_cast = [=]() -> char {
        SYS_ASSERT(
            isascii(optopt));
        return Num::integer_cast<char>(optopt);
    };

    opt_t opt;
    opterr = 0;
    optind = 1;
    while ((opt = getopt_long(
        argc, argv, &shorts[0], &longs[0], nullptr)) != EOF) {
        switch (opt) {
        case opt_type_t::node_type:
            node_type = parse_node_type_optarg(optarg);
            break;
        case opt_type_t::trie_type:
            trie_type = parse_trie_type_optarg(optarg);
            break;
        case opt_type_t::output_type:
            output_type = parse_output_type_optarg(optarg);
            break;
        case opt_type_t::gen_type:
            gen_type = parse_gen_type_optarg(optarg);
            break;
        case opt_type_t::escape_chars:
            escape_chars = optarg;
            break;
        case opt_type_t::expr_type:
            expr_type = parse_expr_type_optarg(optarg);
            break;
        case opt_type_t::expr_depth:
            expr_depth = parse_expr_depth_optarg(optarg);
            break;
        case opt_type_t::print_attributes:
            print_attributes = true;
            break;
        case opt_type_t::no_print_attributes:
            print_attributes = false;
            break;
        case opt_type_t::print_statistics:
            print_statistics = true;
            break;
        case opt_type_t::no_print_statistics:
            print_statistics = false;
            break;
        case opt_type_t::print_dots:
            print_dots = true;
            break;
        case opt_type_t::no_print_dots:
            print_dots = false;
            break;
#ifdef DEBUG
        case opt_type_t::debug:
            debug = Num::bits_cast<bits::debug>(
                parse_debug_optarg(optarg));
            break;
        case opt_type_t::no_debug:
            debug = 0;
            break;
#endif
        case opt_type_t::dump_options:
            bits.dump = true;
            break;
        case opt_type_t::verbose: {
            size_t n = verbose;
            if (Num::check_bits_cast<
                    bits::verbose>(++ n))
                verbose = n;
            break;
        }
        case opt_type_t::no_verbose:
            verbose = 0;
            break;
        case opt_type_t::version:
            bits.version = true;
            break;
        case 0:
            bits.usage = true;
            break;
        case ':': {
            const char* opt = argv_optind();
            if (opt[0] == '-' && opt[1] == '-')
                missing_opt_arg(opt);
            else
                missing_opt_arg(optopt_cast());
            break;
        }
        case '?':
        default:
            if (optopt == 0)
                invalid_opt(argv_optind());
            else
            if (optopt != '?') {
                char* opt = argv_optind();
                if (opt[0] == '-' && opt[1] == '-') {
                    if (char* end = strchr(opt, '='))
                        *end = '\0';
                    not_allowed_opt_arg(opt);
                }
                else
                    invalid_opt(optopt_cast());
            }
            else
                bits.usage = true;
            break;
        }
    }

    this->argc = argc - optind;
    this->argv = argv + optind;

    if (bits.version)
        std::cout
            << program << ": version: " << verdate << "\n\n" << license;

    if (bits.dump) {
        static char const* noyes[] = {
            "no", "yes"
        };

        using namespace Ext;
        std::cout
            << "node-type:    " << array(node_types)[node_type] << '\n'
            << "trie-type:    " << array(trie_types)[trie_type] << '\n'
            << "output-type:  " << array(output_types)[output_type] << '\n'
            << "gen-type:     " << array(gen_types)[gen_type] << '\n'
            << "escape-chars: " << cond(escape_chars, escape_chars, '-') << '\n'
            << "expr-type:    " << array(expr_types)[expr_type] << '\n'
            << "expr-depth:   " << expr_depth << '\n'
            << "print-attrs:  " << array(noyes)[print_attributes] << '\n'
            << "print-stats:  " << array(noyes)[print_statistics] << '\n'
            << "print-dots:   " << array(noyes)[print_dots] << '\n'
#ifdef DEBUG
            << "debug:        " << debug << '\n'
#endif
            << "verbose:      " << verbose << '\n'
            << "argc:         " << this->argc << '\n';

        using namespace std;
        static auto align_diff = [](size_t a) {
            return 14 > a ? 14 - a : 0;
        };
        size_t i = 0;
        for (auto p = this->argv; i < this->argc; i ++, p ++) {
            std::cout
                << "argv["
                << i
                << left
                << setw(align_diff(5 + Num::digits<10>(i)))
                << "]:"
                << cond(*p, repr(*p))
                << '\n';
        }
    }

    if (bits.usage)
      std::cout <<
        "usage: " << program << " [OPTION]...\n"
        "where the options are:\n"
        "  -t|--node-type=NODE      use the specified node type; any of: i[nt], f[loat] or\n"
        "                             s[tr[ing]]; the default is string\n"
        "  -T|--trie-type=TRIE      use the specified trie type: any of array or ternary;\n"
        "                             the default is ternary\n"
        "  -o|--output-type=OUT     generate output of the specified type: o[bj[ect]],\n"
        "                             c[-code], t[rie] or e[xpr]; the default is object\n"
        "  -g|--gen-type=GEN        when printing output of type c-code, trie or expr, generate\n"
        "                             it according to the specification: c[omp[act]] or w[ide];\n"
        "                             the default is wide\n"
        "  -c|--escape-chars=STR    when printing output of type expr, escape the specified chars\n"
        "  -e|--expr-type=EXPR      when printing output of type expr, generate the expression of\n"
        "                             given type: any of c[xxpy], p[erl] or b[ash]; the default\n"
        "                             is cxxpy\n"
        "  -D|--[expr-]depth=NUM    use the specified maximum depth for the generated expression\n"
        "                             when printing output of type expr\n"
        "  -a|--[no-][print-]attrs  put annotation attributes in structure print outs\n"
        "                             or otherwise do not (default not)\n"
        "  -s|--[no-][print-]stats  print out some statistics information\n"
        "                             or otherwise do not (default not)\n"
        "  -d|--[no-][print-]dots   put indenting dots in structure print outs\n"
        "                             or otherwise do not (default not)\n"
#ifdef DEBUG
        "     --debug[=LEVEL]       do print some debugging output; LEVEL is [0-9], default 1;\n"
        "     --no-debug            do not print debugging output at all (default)\n"
#endif
        "     --dump-opt[ion]s      print options and exit\n"
        "  -V|--[no-]verbose        be verbose (cumulative) or not (default not)\n"
        "  -v|--version             print version numbers and exit\n"
        "     --help                display this help info and exit\n";

    if (bits.dump ||
        bits.version ||
        bits.usage)
        exit(0);

    globals = this;
}

} // namespace Opt


