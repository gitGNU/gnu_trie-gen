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

#include "trie.hpp"

namespace Trie {

void error(const char* msg, ...)
{
    STATIC_ASSERT(
        CONFIG_VA_END_NOOP);
    va_list args;
    va_start(args, msg);
    Sys::verror<Error>(msg, args);
    //!!!VA_END va_end(args);
}

void empty_key_error()
{ error("tries cannot contain empty keys"); }

} // namespace Trie


