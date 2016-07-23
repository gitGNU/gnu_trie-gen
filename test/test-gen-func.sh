#!/bin/bash

# Copyright (C) 2016  Stefan Vargyas
# 
# This file is part of Trie-Gen.
# 
# Trie-Gen is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Trie-Gen is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Trie-Gen.  If not, see <http://www.gnu.org/licenses/>.

#
# File generated by a command like:
# $ gen-test -T gen-func
#

[[ "$1" =~ ^-u[0-9]+$ ]] &&
u="${1:2}" ||
u=""

diff -u$u -L gen-func.old <(echo \
'$ print() { printf '\''%s\n'\'' "$@"; }
$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }
$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }
$ . ../commands.sh
$ print '\''i int =integer'\'' '\''f float =floating_point'\'' '\''s str string'\''|gen-func -h .. -f options_t::lookup_node -r node_type_t -v
bool options_t::lookup_node(const char* n, node_type_t& t)
{
    // pattern: f[loat]|i[nt]|s[tr[ing]]
    switch (*n ++) {
    case '\''f'\'':
        if (*n == 0) {
            // input: "f"
            t = node_type_t::floating_point;
            return true;
        }
        if (*n ++ == '\''l'\'' &&
            *n ++ == '\''o'\'' &&
            *n ++ == '\''a'\'' &&
            *n ++ == '\''t'\'' &&
            *n == 0) {
            // input: "float"
            t = node_type_t::floating_point;
            return true;
        }
        return false;
    case '\''i'\'':
        if (*n == 0) {
            // input: "i"
            t = node_type_t::integer;
            return true;
        }
        if (*n ++ == '\''n'\'' &&
            *n ++ == '\''t'\'' &&
            *n == 0) {
            // input: "int"
            t = node_type_t::integer;
            return true;
        }
        return false;
    case '\''s'\'':
        if (*n == 0) {
            // input: "s"
            t = node_type_t::string;
            return true;
        }
        if (*n ++ == '\''t'\'' &&
            *n ++ == '\''r'\'') {
            if (*n == 0) {
                // input: "str"
                t = node_type_t::string;
                return true;
            }
            if (*n ++ == '\''i'\'' &&
                *n ++ == '\''n'\'' &&
                *n ++ == '\''g'\'' &&
                *n == 0) {
                // input: "string"
                t = node_type_t::string;
                return true;
            }
        }
    }
    return false;
}
$ print '\''a array'\'' '\''t ternary'\''|gen-func -h .. -f options_t::lookup_trie -r trie_type_t -v
bool options_t::lookup_trie(const char* n, trie_type_t& t)
{
    // pattern: a[rray]|t[ernary]
    switch (*n ++) {
    case '\''a'\'':
        if (*n == 0) {
            // input: "a"
            t = trie_type_t::array;
            return true;
        }
        if (*n ++ == '\''r'\'' &&
            *n ++ == '\''r'\'' &&
            *n ++ == '\''a'\'' &&
            *n ++ == '\''y'\'' &&
            *n == 0) {
            // input: "array"
            t = trie_type_t::array;
            return true;
        }
        return false;
    case '\''t'\'':
        if (*n == 0) {
            // input: "t"
            t = trie_type_t::ternary;
            return true;
        }
        if (*n ++ == '\''e'\'' &&
            *n ++ == '\''r'\'' &&
            *n ++ == '\''n'\'' &&
            *n ++ == '\''a'\'' &&
            *n ++ == '\''r'\'' &&
            *n ++ == '\''y'\'' &&
            *n == 0) {
            // input: "ternary"
            t = trie_type_t::ternary;
            return true;
        }
    }
    return false;
}
$ print '\''o obj object'\'' '\''c c-code =c_code'\'' '\''t trie'\'' '\''e expr'\''|gen-func -h .. -f options_t::lookup_output -r output_type_t -v
bool options_t::lookup_output(const char* n, output_type_t& t)
{
    // pattern: c[-code]|e[xpr]|o[bj[ect]]|t[rie]
    switch (*n ++) {
    case '\''c'\'':
        if (*n == 0) {
            // input: "c"
            t = output_type_t::c_code;
            return true;
        }
        if (*n ++ == '\''-'\'' &&
            *n ++ == '\''c'\'' &&
            *n ++ == '\''o'\'' &&
            *n ++ == '\''d'\'' &&
            *n ++ == '\''e'\'' &&
            *n == 0) {
            // input: "c-code"
            t = output_type_t::c_code;
            return true;
        }
        return false;
    case '\''e'\'':
        if (*n == 0) {
            // input: "e"
            t = output_type_t::expr;
            return true;
        }
        if (*n ++ == '\''x'\'' &&
            *n ++ == '\''p'\'' &&
            *n ++ == '\''r'\'' &&
            *n == 0) {
            // input: "expr"
            t = output_type_t::expr;
            return true;
        }
        return false;
    case '\''o'\'':
        if (*n == 0) {
            // input: "o"
            t = output_type_t::object;
            return true;
        }
        if (*n ++ == '\''b'\'' &&
            *n ++ == '\''j'\'') {
            if (*n == 0) {
                // input: "obj"
                t = output_type_t::object;
                return true;
            }
            if (*n ++ == '\''e'\'' &&
                *n ++ == '\''c'\'' &&
                *n ++ == '\''t'\'' &&
                *n == 0) {
                // input: "object"
                t = output_type_t::object;
                return true;
            }
        }
        return false;
    case '\''t'\'':
        if (*n == 0) {
            // input: "t"
            t = output_type_t::trie;
            return true;
        }
        if (*n ++ == '\''r'\'' &&
            *n ++ == '\''i'\'' &&
            *n ++ == '\''e'\'' &&
            *n == 0) {
            // input: "trie"
            t = output_type_t::trie;
            return true;
        }
    }
    return false;
}
$ print '\''c comp compact'\'' '\''w wide'\''|gen-func -h .. -f options_t::lookup_gen -r gen_type_t -v
bool options_t::lookup_gen(const char* n, gen_type_t& t)
{
    // pattern: c[omp[act]]|w[ide]
    switch (*n ++) {
    case '\''c'\'':
        if (*n == 0) {
            // input: "c"
            t = gen_type_t::compact;
            return true;
        }
        if (*n ++ == '\''o'\'' &&
            *n ++ == '\''m'\'' &&
            *n ++ == '\''p'\'') {
            if (*n == 0) {
                // input: "comp"
                t = gen_type_t::compact;
                return true;
            }
            if (*n ++ == '\''a'\'' &&
                *n ++ == '\''c'\'' &&
                *n ++ == '\''t'\'' &&
                *n == 0) {
                // input: "compact"
                t = gen_type_t::compact;
                return true;
            }
        }
        return false;
    case '\''w'\'':
        if (*n == 0) {
            // input: "w"
            t = gen_type_t::wide;
            return true;
        }
        if (*n ++ == '\''i'\'' &&
            *n ++ == '\''d'\'' &&
            *n ++ == '\''e'\'' &&
            *n == 0) {
            // input: "wide"
            t = gen_type_t::wide;
            return true;
        }
    }
    return false;
}
$ print '\''c cxxpy'\'' '\''p perl'\'' '\''b bash'\''|gen-func -h .. -f options_t::lookup_expr -r expr_type_t -v
bool options_t::lookup_expr(const char* n, expr_type_t& t)
{
    // pattern: b[ash]|c[xxpy]|p[erl]
    switch (*n ++) {
    case '\''b'\'':
        if (*n == 0) {
            // input: "b"
            t = expr_type_t::bash;
            return true;
        }
        if (*n ++ == '\''a'\'' &&
            *n ++ == '\''s'\'' &&
            *n ++ == '\''h'\'' &&
            *n == 0) {
            // input: "bash"
            t = expr_type_t::bash;
            return true;
        }
        return false;
    case '\''c'\'':
        if (*n == 0) {
            // input: "c"
            t = expr_type_t::cxxpy;
            return true;
        }
        if (*n ++ == '\''x'\'' &&
            *n ++ == '\''x'\'' &&
            *n ++ == '\''p'\'' &&
            *n ++ == '\''y'\'' &&
            *n == 0) {
            // input: "cxxpy"
            t = expr_type_t::cxxpy;
            return true;
        }
        return false;
    case '\''p'\'':
        if (*n == 0) {
            // input: "p"
            t = expr_type_t::perl;
            return true;
        }
        if (*n ++ == '\''e'\'' &&
            *n ++ == '\''r'\'' &&
            *n ++ == '\''l'\'' &&
            *n == 0) {
            // input: "perl"
            t = expr_type_t::perl;
            return true;
        }
    }
    return false;
}'
) -L gen-func.new <(
echo '$ print() { printf '\''%s\n'\'' "$@"; }'
print() { printf '%s\n' "$@"; } 2>&1 ||
echo 'command failed: print() { printf '\''%s\n'\'' "$@"; }'

echo '$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'
printc() { awk '{ printf("%c\n", strtonum("0x" $1)) }' <<< "$1"; } 2>&1 ||
echo 'command failed: printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'

echo '$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'
trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r 's|^#|\\\0|'; } 2>&1 ||
echo 'command failed: trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'

echo '$ . ../commands.sh'
. ../commands.sh 2>&1 ||
echo 'command failed: . ../commands.sh'

echo '$ print '\''i int =integer'\'' '\''f float =floating_point'\'' '\''s str string'\''|gen-func -h .. -f options_t::lookup_node -r node_type_t -v'
print 'i int =integer' 'f float =floating_point' 's str string'|gen-func -h .. -f options_t::lookup_node -r node_type_t -v 2>&1 ||
echo 'command failed: print '\''i int =integer'\'' '\''f float =floating_point'\'' '\''s str string'\''|gen-func -h .. -f options_t::lookup_node -r node_type_t -v'

echo '$ print '\''a array'\'' '\''t ternary'\''|gen-func -h .. -f options_t::lookup_trie -r trie_type_t -v'
print 'a array' 't ternary'|gen-func -h .. -f options_t::lookup_trie -r trie_type_t -v 2>&1 ||
echo 'command failed: print '\''a array'\'' '\''t ternary'\''|gen-func -h .. -f options_t::lookup_trie -r trie_type_t -v'

echo '$ print '\''o obj object'\'' '\''c c-code =c_code'\'' '\''t trie'\'' '\''e expr'\''|gen-func -h .. -f options_t::lookup_output -r output_type_t -v'
print 'o obj object' 'c c-code =c_code' 't trie' 'e expr'|gen-func -h .. -f options_t::lookup_output -r output_type_t -v 2>&1 ||
echo 'command failed: print '\''o obj object'\'' '\''c c-code =c_code'\'' '\''t trie'\'' '\''e expr'\''|gen-func -h .. -f options_t::lookup_output -r output_type_t -v'

echo '$ print '\''c comp compact'\'' '\''w wide'\''|gen-func -h .. -f options_t::lookup_gen -r gen_type_t -v'
print 'c comp compact' 'w wide'|gen-func -h .. -f options_t::lookup_gen -r gen_type_t -v 2>&1 ||
echo 'command failed: print '\''c comp compact'\'' '\''w wide'\''|gen-func -h .. -f options_t::lookup_gen -r gen_type_t -v'

echo '$ print '\''c cxxpy'\'' '\''p perl'\'' '\''b bash'\''|gen-func -h .. -f options_t::lookup_expr -r expr_type_t -v'
print 'c cxxpy' 'p perl' 'b bash'|gen-func -h .. -f options_t::lookup_expr -r expr_type_t -v 2>&1 ||
echo 'command failed: print '\''c cxxpy'\'' '\''p perl'\'' '\''b bash'\''|gen-func -h .. -f options_t::lookup_expr -r expr_type_t -v'
)
