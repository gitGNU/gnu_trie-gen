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

#include <iostream>
#include <string>

#include "sys.hpp"
#include "num.hpp"
#include "ext.hpp"
#include "glob.hpp"
#include "trie.hpp"
#include "opt.hpp"

#include "demangle.hpp"

const char program[] = STRINGIFY(PROGRAM);
const char verdate[] = "v0.5 2016-07-20"; // $ date +%F

const char license[] =
"Copyright (C) 2016  Stefan Vargyas.\n"
"License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n"
"This is free software: you are free to change and redistribute it.\n"
"There is NO WARRANTY, to the extent permitted by law.\n";

const global_options_t* globals = nullptr;

typedef Opt::options_t::output_type_t output_type_t;
typedef Opt::options_t::gen_type_t gen_type_t;

template<typename T>
void output_trie(
    const T& trie, std::ostream& ost,
    output_type_t out_type,
    gen_type_t gen_type)
{
    typedef T trie_t;
    static void (trie_t::*const gen_funcs[])(
        std::ostream&, Trie::gen_type_t) const = {
        nullptr,           // output_type_t::object
        &trie_t::gen_code, // output_type_t::c_code
        &trie_t::gen_trie, // output_type_t::trie
        &trie_t::gen_expr, // output_type_t::expr
    };
    const auto trie_gen = [=]() -> Trie::gen_type_t {
        switch (gen_type) {
        case gen_type_t::compact:
            return Trie::gen_type_t::compact;
        case gen_type_t::wide:
            return Trie::gen_type_t::wide;
        default:
            SYS_UNEXPECT_ERR(
                "unknown gen-type: %zu",
                Num::integer_cast<size_t>(
                    gen_type));
        }
    };
    using namespace Ext;
    using namespace Trie;
    if (globals->verbose > 1)
        ost << "# final trie:\n";
    if (globals->print_statistics)
        trie.stat(ost);
    ost << (out_type != output_type_t::object
            ? manip(array(gen_funcs)[out_type],
                &trie, trie_gen())
            : print(trie));
}

template<typename T>
class exec_value_t
{
public:
    typedef
        typename std::enable_if<
            Num::is_arithmetic<T>(), T>::type
        type_t;

    type_t operator()(const std::string& key UNUSED)
    { return ++ n; }

private:
    type_t n = 0;
};

template<>
struct exec_value_t<std::string>
{
    typedef const std::string& type_t;

    type_t operator()(const std::string& key)
    { return key; }
};

template<typename T>
void exec_trie(
    std::istream& ist,
    std::ostream& ost,
    output_type_t out_type,
    gen_type_t gen_type)
{
    using namespace std;
    using namespace Ext;

    typedef T trie_t;

    typedef exec_value_t<
        typename trie_t::value_t> exec_t;
    typedef
        typename exec_t::type_t val_t;

    string str;
    exec_t exec;
    trie_t trie;
    while (getline(ist, str)) {
        if (globals->verbose > 1)
            ost << "# adding "
                << repr(str)
                << ":\n";
        if (globals->verbose > 1 &&
            globals->print_statistics)
            trie.stat(ost);
        if (globals->verbose > 1)
            ost << trie;
        val_t val = exec(str);
        trie.put(str, val);
#if defined(DEBUG) && \
    defined(DEBUG_TRIE_GET)
        auto res = trie.get(str);
        SYS_ASSERT(
            res != nullptr &&
            val == *res);
#endif
    }

    output_trie(
        trie, ost,
        out_type,
        gen_type);
}

int main(int argc, char** argv)
try
{
    auto opts = Opt::parse_options(argc, argv);

    static void (*const execs[][3])(
        std::istream&, std::ostream&, output_type_t, gen_type_t) = {{
        // trie_type_t::array_trie
        exec_trie<Trie::ArrayTrie<int>>,           // node_type_t::integer
        exec_trie<Trie::ArrayTrie<double>>,        // node_type_t::floating_point
        exec_trie<Trie::ArrayTrie<std::string>>,   // node_type_t::string
    },{
        // trie_type_t::ternary_trie
        exec_trie<Trie::TernaryTrie<int>>,         // node_type_t::integer
        exec_trie<Trie::TernaryTrie<double>>,      // node_type_t::floating_point
        exec_trie<Trie::TernaryTrie<std::string>>, // node_type_t::string
    }};
    Ext::array(execs)[opts->trie_type][opts->node_type](
        std::cin, std::cout,
        opts->output_type,
        opts->gen_type);

    return 0;
}
catch (const Sys::Error& err) {
    std::cerr
        << program
        << ": error: "
        << err.what()
        << std::endl;
    return 1;
}
catch (const std::exception& exc) {
    std::cerr
        << program
        << ": exception: "
        << Sys::demangle(exc)
        << ": "
        << exc.what()
        << std::endl;
    return 1;
}
catch (...) {
    std::cerr
        << program
        << ": error: unknown exception"
        << std::endl;
    return 1;
}


