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

#include <cstdio>
#include <cstdarg>
#include <iostream>

#include "sys.hpp"

namespace Sys {

static void die(const char* msg, ...)
    PRINTF_FMT(1)
    NORETURN;

static void die(const char* msg, ...)
{
    char buf[256];

    va_list args;
    va_start(args, msg);
    vsnprintf(buf, sizeof buf - 1, msg, args);
    va_end(args);
    buf[255] = 0;

    std::cerr << program << ": fatal error: " << buf << std::endl;

    exit(127);
}

void assert_failed(
    const char* file, int line, const char* func, const char* expr)
{
    char buf[256];

    snprintf(
        buf, sizeof buf - 1,
        "%s:%d:%s: %s", file, line, func, expr);
    buf[255] = 0;

    die("assertion failed: %s", buf);
}

void unexpect_error(
    const char* file, int line, const char* func, const char* msg, ...)
{
    char buf[256];

    va_list args;
    va_start(args, msg);
    vsnprintf(buf, sizeof buf - 1, msg, args);
    va_end(args);
    buf[255] = 0;

    die("unexpected error: %s:%d:%s: %s", file, line, func, buf);
}

} // namespace Sys


