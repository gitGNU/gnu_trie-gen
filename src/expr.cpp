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

#include "ext.hpp"
#include "expr.hpp"

namespace Expr {

const expr_def_t* expr_def_t::get_def(expr_type_t type)
{
    static const expr_def_t cxxpy_def = {
        "()[]*+|",
        "(", ")", "[", "]", "|", true
    };
    static const expr_def_t perl_def = {
        "[](){}^$|.,?*+",
        "(?:", ")", "(?:", ")?", "|", true
    };
    static const expr_def_t bash_def = {
        "{},*?",
        "{", "}", "{,", "}", ",", false
    };
    static const expr_def_t* const defs[] = {
        &cxxpy_def, // expr_type_t::cxxpy
        &perl_def,  // expr_type_t::perl
        &bash_def,  // expr_type_t::bash
    };
    return Ext::array(defs)[type];
}

} // namespace Expr


