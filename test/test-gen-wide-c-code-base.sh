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
# $ gen-test -T gen-wide-c-code-base
#

[[ "$1" =~ ^-u[0-9]+$ ]] &&
u="${1:2}" ||
u=""

diff -u$u -L gen-wide-c-code-base.old <(echo \
'$ print() { printf '\''%s\n'\'' "$@"; }
$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }
$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }
$ trie() { ../src/trie --gen=wide --output=c-code --attrs --no-dots "$@"; }
$ print a ab abc abd ac|trie
    // [1,'\''a'\'']
    // '\''a'\'' [2,'\''c'\'']
    if (*p ++ == '\''a'\'') {
        if (*p == 0)
            return "a";
        switch (*p ++) {
        // '\''b'\'' [2,'\''d'\'']
        case '\''b'\'':
            if (*p == 0)
                return "ab";
            switch (*p ++) {
            // '\''c'\'' [0]
            case '\''c'\'':
                if (*p == 0)
                    return "abc";
                return error;
            // '\''d'\'' [0]
            case '\''d'\'':
                if (*p == 0)
                    return "abd";
            }
            return error;
        // '\''c'\'' [0]
        case '\''c'\'':
            if (*p == 0)
                return "ac";
        }
    }
    return error;
$ print a ab abc abd ac|trie -t int
    // [1,'\''a'\'']
    // '\''a'\'' [2,'\''c'\'']
    if (*p ++ == '\''a'\'') {
        if (*p == 0)
            return 1;
        switch (*p ++) {
        // '\''b'\'' [2,'\''d'\'']
        case '\''b'\'':
            if (*p == 0)
                return 2;
            switch (*p ++) {
            // '\''c'\'' [0]
            case '\''c'\'':
                if (*p == 0)
                    return 3;
                return error;
            // '\''d'\'' [0]
            case '\''d'\'':
                if (*p == 0)
                    return 4;
            }
            return error;
        // '\''c'\'' [0]
        case '\''c'\'':
            if (*p == 0)
                return 5;
        }
    }
    return error;
$ print a ab abc abd ac|trie -t float
    // [1,'\''a'\'']
    // '\''a'\'' [2,'\''c'\'']
    if (*p ++ == '\''a'\'') {
        if (*p == 0)
            return 1.0;
        switch (*p ++) {
        // '\''b'\'' [2,'\''d'\'']
        case '\''b'\'':
            if (*p == 0)
                return 2.0;
            switch (*p ++) {
            // '\''c'\'' [0]
            case '\''c'\'':
                if (*p == 0)
                    return 3.0;
                return error;
            // '\''d'\'' [0]
            case '\''d'\'':
                if (*p == 0)
                    return 4.0;
            }
            return error;
        // '\''c'\'' [0]
        case '\''c'\'':
            if (*p == 0)
                return 5.0;
        }
    }
    return error;'
) -L gen-wide-c-code-base.new <(
echo '$ print() { printf '\''%s\n'\'' "$@"; }'
print() { printf '%s\n' "$@"; } 2>&1 ||
echo 'command failed: print() { printf '\''%s\n'\'' "$@"; }'

echo '$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'
printc() { awk '{ printf("%c\n", strtonum("0x" $1)) }' <<< "$1"; } 2>&1 ||
echo 'command failed: printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'

echo '$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'
trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r 's|^#|\\\0|'; } 2>&1 ||
echo 'command failed: trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'

echo '$ trie() { ../src/trie --gen=wide --output=c-code --attrs --no-dots "$@"; }'
trie() { ../src/trie --gen=wide --output=c-code --attrs --no-dots "$@"; } 2>&1 ||
echo 'command failed: trie() { ../src/trie --gen=wide --output=c-code --attrs --no-dots "$@"; }'

echo '$ print a ab abc abd ac|trie'
print a ab abc abd ac|trie 2>&1 ||
echo 'command failed: print a ab abc abd ac|trie'

echo '$ print a ab abc abd ac|trie -t int'
print a ab abc abd ac|trie -t int 2>&1 ||
echo 'command failed: print a ab abc abd ac|trie -t int'

echo '$ print a ab abc abd ac|trie -t float'
print a ab abc abd ac|trie -t float 2>&1 ||
echo 'command failed: print a ab abc abd ac|trie -t float'
)

