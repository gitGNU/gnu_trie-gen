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
# $ gen-test -T dup
#

[[ "$1" =~ ^-u[0-9]+$ ]] &&
u="${1:2}" ||
u=""

diff -u$u -L dup.old <(echo \
'$ print() { printf '\''%s\n'\'' "$@"; }
$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }
$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }
$ trie() { trie0 --node=int --stats -VV "$@"; }
$ print a a|trie --trie=array
\# adding "a":
\# node size:  2072
\# node count: 1
\# nodes size: 2072
\# adding "a":
\# node size:  2072
\# node count: 2
\# nodes size: 4144
{
    '\''a'\'': 1
}
\# final trie:
\# node size:  2072
\# node count: 2
\# nodes size: 4144
{
    '\''a'\'': 2
}
$ print a a|trie --trie=ternary
\# adding "a":
\# node size:  48
\# node count: 0
\# nodes size: 0
\# adding "a":
\# node size:  48
\# node count: 2
\# nodes size: 96
{
    .ch: '\''a'\''
    .eq {
        .val: 1
    }
}
\# final trie:
\# node size:  48
\# node count: 2
\# nodes size: 96
{
    .ch: '\''a'\''
    .eq {
        .val: 2
    }
}'
) -L dup.new <(
echo '$ print() { printf '\''%s\n'\'' "$@"; }'
print() { printf '%s\n' "$@"; } 2>&1 ||
echo 'command failed: print() { printf '\''%s\n'\'' "$@"; }'

echo '$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'
printc() { awk '{ printf("%c\n", strtonum("0x" $1)) }' <<< "$1"; } 2>&1 ||
echo 'command failed: printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'

echo '$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'
trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r 's|^#|\\\0|'; } 2>&1 ||
echo 'command failed: trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'

echo '$ trie() { trie0 --node=int --stats -VV "$@"; }'
trie() { trie0 --node=int --stats -VV "$@"; } 2>&1 ||
echo 'command failed: trie() { trie0 --node=int --stats -VV "$@"; }'

echo '$ print a a|trie --trie=array'
print a a|trie --trie=array 2>&1 ||
echo 'command failed: print a a|trie --trie=array'

echo '$ print a a|trie --trie=ternary'
print a a|trie --trie=ternary 2>&1 ||
echo 'command failed: print a a|trie --trie=ternary'
)

