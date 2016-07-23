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
# $ gen-test -T base
#

[[ "$1" =~ ^-u[0-9]+$ ]] &&
u="${1:2}" ||
u=""

diff -u$u -L base.old <(echo \
'$ print() { printf '\''%s\n'\'' "$@"; }
$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }
$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }
$ trie() { ../src/trie --trie=array --dots "$@"; }
$ print test-str test-string test-int test-float|trie --node=int
{
.   '\''t'\'' {
.   .   '\''e'\'' {
.   .   .   '\''s'\'' {
.   .   .   .   '\''t'\'' {
.   .   .   .   .   '\''-'\'' {
.   .   .   .   .   .   '\''f'\'' {
.   .   .   .   .   .   .   '\''l'\'' {
.   .   .   .   .   .   .   .   '\''o'\'' {
.   .   .   .   .   .   .   .   .   '\''a'\'' {
.   .   .   .   .   .   .   .   .   .   '\''t'\'': 4
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   .   '\''i'\'' {
.   .   .   .   .   .   .   '\''n'\'' {
.   .   .   .   .   .   .   .   '\''t'\'': 3
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   .   '\''s'\'' {
.   .   .   .   .   .   .   '\''t'\'' {
.   .   .   .   .   .   .   .   '\''r'\'': 1 {
.   .   .   .   .   .   .   .   .   '\''i'\'' {
.   .   .   .   .   .   .   .   .   .   '\''n'\'' {
.   .   .   .   .   .   .   .   .   .   .   '\''g'\'': 2
.   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   }
.   .   .   .   }
.   .   .   }
.   .   }
.   }
}
$ print test-str test-string test-int test-float|trie --node=float
{
.   '\''t'\'' {
.   .   '\''e'\'' {
.   .   .   '\''s'\'' {
.   .   .   .   '\''t'\'' {
.   .   .   .   .   '\''-'\'' {
.   .   .   .   .   .   '\''f'\'' {
.   .   .   .   .   .   .   '\''l'\'' {
.   .   .   .   .   .   .   .   '\''o'\'' {
.   .   .   .   .   .   .   .   .   '\''a'\'' {
.   .   .   .   .   .   .   .   .   .   '\''t'\'': 4.0
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   .   '\''i'\'' {
.   .   .   .   .   .   .   '\''n'\'' {
.   .   .   .   .   .   .   .   '\''t'\'': 3.0
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   .   '\''s'\'' {
.   .   .   .   .   .   .   '\''t'\'' {
.   .   .   .   .   .   .   .   '\''r'\'': 1.0 {
.   .   .   .   .   .   .   .   .   '\''i'\'' {
.   .   .   .   .   .   .   .   .   .   '\''n'\'' {
.   .   .   .   .   .   .   .   .   .   .   '\''g'\'': 2.0
.   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   }
.   .   .   .   }
.   .   .   }
.   .   }
.   }
}
$ print test-str test-string test-int test-float|trie --node=str
{
.   '\''t'\'' {
.   .   '\''e'\'' {
.   .   .   '\''s'\'' {
.   .   .   .   '\''t'\'' {
.   .   .   .   .   '\''-'\'' {
.   .   .   .   .   .   '\''f'\'' {
.   .   .   .   .   .   .   '\''l'\'' {
.   .   .   .   .   .   .   .   '\''o'\'' {
.   .   .   .   .   .   .   .   .   '\''a'\'' {
.   .   .   .   .   .   .   .   .   .   '\''t'\'': "test-float"
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   .   '\''i'\'' {
.   .   .   .   .   .   .   '\''n'\'' {
.   .   .   .   .   .   .   .   '\''t'\'': "test-int"
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   .   '\''s'\'' {
.   .   .   .   .   .   .   '\''t'\'' {
.   .   .   .   .   .   .   .   '\''r'\'': "test-str" {
.   .   .   .   .   .   .   .   .   '\''i'\'' {
.   .   .   .   .   .   .   .   .   .   '\''n'\'' {
.   .   .   .   .   .   .   .   .   .   .   '\''g'\'': "test-string"
.   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   }
.   .   .   .   }
.   .   .   }
.   .   }
.   }
}
$ trie() { ../src/trie --trie=ternary --dots "$@"; }
$ print test-str test-string test-int test-float|trie --node=int
{
.   .ch: '\''t'\''
.   .eq {
.   .   .ch: '\''e'\''
.   .   .eq {
.   .   .   .ch: '\''s'\''
.   .   .   .eq {
.   .   .   .   .ch: '\''t'\''
.   .   .   .   .eq {
.   .   .   .   .   .ch: '\''-'\''
.   .   .   .   .   .eq {
.   .   .   .   .   .   .ch: '\''s'\''
.   .   .   .   .   .   .lo {
.   .   .   .   .   .   .   .ch: '\''i'\''
.   .   .   .   .   .   .   .lo {
.   .   .   .   .   .   .   .   .ch: '\''f'\''
.   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .ch: '\''l'\''
.   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .ch: '\''o'\''
.   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .ch: '\''a'\''
.   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .ch: '\''t'\''
.   .   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .   .val: 4
.   .   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .ch: '\''n'\''
.   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .ch: '\''t'\''
.   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .val: 3
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .ch: '\''t'\''
.   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .ch: '\''r'\''
.   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .val: 1
.   .   .   .   .   .   .   .   .   .hi {
.   .   .   .   .   .   .   .   .   .   .ch: '\''i'\''
.   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .ch: '\''n'\''
.   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .ch: '\''g'\''
.   .   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .   .val: 2
.   .   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   }
.   .   .   .   }
.   .   .   }
.   .   }
.   }
}
$ print test-str test-string test-int test-float|trie --node=float
{
.   .ch: '\''t'\''
.   .eq {
.   .   .ch: '\''e'\''
.   .   .eq {
.   .   .   .ch: '\''s'\''
.   .   .   .eq {
.   .   .   .   .ch: '\''t'\''
.   .   .   .   .eq {
.   .   .   .   .   .ch: '\''-'\''
.   .   .   .   .   .eq {
.   .   .   .   .   .   .ch: '\''s'\''
.   .   .   .   .   .   .lo {
.   .   .   .   .   .   .   .ch: '\''i'\''
.   .   .   .   .   .   .   .lo {
.   .   .   .   .   .   .   .   .ch: '\''f'\''
.   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .ch: '\''l'\''
.   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .ch: '\''o'\''
.   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .ch: '\''a'\''
.   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .ch: '\''t'\''
.   .   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .   .val: 4.0
.   .   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .ch: '\''n'\''
.   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .ch: '\''t'\''
.   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .val: 3.0
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .ch: '\''t'\''
.   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .ch: '\''r'\''
.   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .val: 1.0
.   .   .   .   .   .   .   .   .   .hi {
.   .   .   .   .   .   .   .   .   .   .ch: '\''i'\''
.   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .ch: '\''n'\''
.   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .ch: '\''g'\''
.   .   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .   .val: 2.0
.   .   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   }
.   .   .   .   }
.   .   .   }
.   .   }
.   }
}
$ print test-str test-string test-int test-float|trie --node=str
{
.   .ch: '\''t'\''
.   .eq {
.   .   .ch: '\''e'\''
.   .   .eq {
.   .   .   .ch: '\''s'\''
.   .   .   .eq {
.   .   .   .   .ch: '\''t'\''
.   .   .   .   .eq {
.   .   .   .   .   .ch: '\''-'\''
.   .   .   .   .   .eq {
.   .   .   .   .   .   .ch: '\''s'\''
.   .   .   .   .   .   .lo {
.   .   .   .   .   .   .   .ch: '\''i'\''
.   .   .   .   .   .   .   .lo {
.   .   .   .   .   .   .   .   .ch: '\''f'\''
.   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .ch: '\''l'\''
.   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .ch: '\''o'\''
.   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .ch: '\''a'\''
.   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .ch: '\''t'\''
.   .   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .   .val: "test-float"
.   .   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .ch: '\''n'\''
.   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .ch: '\''t'\''
.   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .val: "test-int"
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .ch: '\''t'\''
.   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .ch: '\''r'\''
.   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .val: "test-str"
.   .   .   .   .   .   .   .   .   .hi {
.   .   .   .   .   .   .   .   .   .   .ch: '\''i'\''
.   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .ch: '\''n'\''
.   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .ch: '\''g'\''
.   .   .   .   .   .   .   .   .   .   .   .   .eq {
.   .   .   .   .   .   .   .   .   .   .   .   .   .val: "test-string"
.   .   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   .   }
.   .   .   .   .   .   .   }
.   .   .   .   .   .   }
.   .   .   .   .   }
.   .   .   .   }
.   .   .   }
.   .   }
.   }
}'
) -L base.new <(
echo '$ print() { printf '\''%s\n'\'' "$@"; }'
print() { printf '%s\n' "$@"; } 2>&1 ||
echo 'command failed: print() { printf '\''%s\n'\'' "$@"; }'

echo '$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'
printc() { awk '{ printf("%c\n", strtonum("0x" $1)) }' <<< "$1"; } 2>&1 ||
echo 'command failed: printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'

echo '$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'
trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r 's|^#|\\\0|'; } 2>&1 ||
echo 'command failed: trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'

echo '$ trie() { ../src/trie --trie=array --dots "$@"; }'
trie() { ../src/trie --trie=array --dots "$@"; } 2>&1 ||
echo 'command failed: trie() { ../src/trie --trie=array --dots "$@"; }'

echo '$ print test-str test-string test-int test-float|trie --node=int'
print test-str test-string test-int test-float|trie --node=int 2>&1 ||
echo 'command failed: print test-str test-string test-int test-float|trie --node=int'

echo '$ print test-str test-string test-int test-float|trie --node=float'
print test-str test-string test-int test-float|trie --node=float 2>&1 ||
echo 'command failed: print test-str test-string test-int test-float|trie --node=float'

echo '$ print test-str test-string test-int test-float|trie --node=str'
print test-str test-string test-int test-float|trie --node=str 2>&1 ||
echo 'command failed: print test-str test-string test-int test-float|trie --node=str'

echo '$ trie() { ../src/trie --trie=ternary --dots "$@"; }'
trie() { ../src/trie --trie=ternary --dots "$@"; } 2>&1 ||
echo 'command failed: trie() { ../src/trie --trie=ternary --dots "$@"; }'

echo '$ print test-str test-string test-int test-float|trie --node=int'
print test-str test-string test-int test-float|trie --node=int 2>&1 ||
echo 'command failed: print test-str test-string test-int test-float|trie --node=int'

echo '$ print test-str test-string test-int test-float|trie --node=float'
print test-str test-string test-int test-float|trie --node=float 2>&1 ||
echo 'command failed: print test-str test-string test-int test-float|trie --node=float'

echo '$ print test-str test-string test-int test-float|trie --node=str'
print test-str test-string test-int test-float|trie --node=str 2>&1 ||
echo 'command failed: print test-str test-string test-int test-float|trie --node=str'
)
