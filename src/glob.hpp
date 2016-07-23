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

#ifndef __GLOB_HPP
#define __GLOB_HPP

struct global_options_t
{
    struct bits {
        enum {
#ifdef DEBUG
            debug      = 4,
#endif
            print_dots = 1,
            print_attributes = 1,
            print_statistics = 1,
            verbose    = 2,
        };
    };

    enum struct expr_type_t {
        cxxpy,
        perl,
        bash,
    };

    const char* escape_chars = nullptr;
    expr_type_t expr_type    = expr_type_t::cxxpy;  
    unsigned    expr_depth   = 0;

#ifdef DEBUG
    unsigned debug      : bits::debug;
#endif
    unsigned print_dots : bits::print_dots;
    unsigned print_attributes
                        : bits::print_attributes;
    unsigned print_statistics
                        : bits::print_statistics;
    unsigned verbose    : bits::verbose;

    global_options_t()
    {
#ifdef DEBUG
        debug = 0;
#endif
        print_dots = 0;
        print_attributes = 0;
        print_statistics = 0;
        verbose = 0;
    }
};

extern const global_options_t* globals;

#endif /* __GLOB_HPP */


