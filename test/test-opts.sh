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
# $ gen-test -T opts
#

[[ "$1" =~ ^-u[0-9]+$ ]] &&
u="${1:2}" ||
u=""

diff -u$u -L opts.old <(echo \
'$ print() { printf '\''%s\n'\'' "$@"; }
$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }
$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }
$ trie() { ../src/trie --dump-opts "$@"; }
$ trie
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --node-type int --node-type string
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --node-type int --node-type str
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --node-type int
node-type:    int
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --node-type float
node-type:    float
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --node-type foo
trie: error: invalid argument for '\''node-type'\'' option: '\''foo'\''
command failed: trie --node-type foo
$ trie --trie-type array --trie-type ternary
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --trie-type array
node-type:    string
trie-type:    array
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --trie-type foo
trie: error: invalid argument for '\''trie-type'\'' option: '\''foo'\''
command failed: trie --trie-type foo
$ trie --output-type c-code --output-type object
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --output-type c-code --output-type obj
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --output-type c-code --output-type o
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --output-type c-code
node-type:    string
trie-type:    ternary
output-type:  c-code
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --output-type c
node-type:    string
trie-type:    ternary
output-type:  c-code
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --output-type foo
trie: error: invalid argument for '\''output-type'\'' option: '\''foo'\''
command failed: trie --output-type foo
$ trie --gen-type wide --gen-type compact
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     compact
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --gen-type wide --gen-type comp
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     compact
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --gen-type wide --gen-type c
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     compact
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --gen-type wide
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --gen-type w
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --gen-type foo
trie: error: invalid argument for '\''gen-type'\'' option: '\''foo'\''
command failed: trie --gen-type foo
$ trie --debug
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        1
verbose:      0
argc:         0
$ trie --debug=0
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --debug=1
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        1
verbose:      0
argc:         0
$ trie --debug=2
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        2
verbose:      0
argc:         0
$ trie --debug=3
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        3
verbose:      0
argc:         0
$ trie --debug=4
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        4
verbose:      0
argc:         0
$ trie --debug=5
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        5
verbose:      0
argc:         0
$ trie --debug=6
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        6
verbose:      0
argc:         0
$ trie --debug=7
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        7
verbose:      0
argc:         0
$ trie --debug=8
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        8
verbose:      0
argc:         0
$ trie --debug=9
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        9
verbose:      0
argc:         0
$ trie --debug=10
trie: error: invalid argument for '\''debug'\'' option: '\''10'\''
command failed: trie --debug=10
$ trie --debug=100
trie: error: invalid argument for '\''debug'\'' option: '\''100'\''
command failed: trie --debug=100
$ trie --debug --no-debug
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --no-debug=
trie: error: option '\''--no-debug'\'' does not allow an argument
command failed: trie --no-debug=
$ trie --no-verbose
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie --verbose
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      1
argc:         0
$ trie --verbose --verbose
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      2
argc:         0
$ trie --verbose --verbose --verbose
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      3
argc:         0
$ trie --verbose --verbose --verbose --verbose
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      3
argc:         0
$ trie --verbose --verbose --verbose --verbose --no-verbose
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      0
argc:         0
$ trie -V
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      1
argc:         0
$ trie -VV
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      2
argc:         0
$ trie -VVV
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      3
argc:         0
$ trie -VVVV
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      3
argc:         0
$ trie -VVVVV
node-type:    string
trie-type:    ternary
output-type:  object
gen-type:     wide
escape-chars: -
expr-type:    cxxpy
expr-depth:   0
print-attrs:  no
print-stats:  no
print-dots:   no
debug:        0
verbose:      3
argc:         0'
) -L opts.new <(
echo '$ print() { printf '\''%s\n'\'' "$@"; }'
print() { printf '%s\n' "$@"; } 2>&1 ||
echo 'command failed: print() { printf '\''%s\n'\'' "$@"; }'

echo '$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'
printc() { awk '{ printf("%c\n", strtonum("0x" $1)) }' <<< "$1"; } 2>&1 ||
echo 'command failed: printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'

echo '$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'
trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r 's|^#|\\\0|'; } 2>&1 ||
echo 'command failed: trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'

echo '$ trie() { ../src/trie --dump-opts "$@"; }'
trie() { ../src/trie --dump-opts "$@"; } 2>&1 ||
echo 'command failed: trie() { ../src/trie --dump-opts "$@"; }'

echo '$ trie'
trie 2>&1 ||
echo 'command failed: trie'

echo '$ trie --node-type int --node-type string'
trie --node-type int --node-type string 2>&1 ||
echo 'command failed: trie --node-type int --node-type string'

echo '$ trie --node-type int --node-type str'
trie --node-type int --node-type str 2>&1 ||
echo 'command failed: trie --node-type int --node-type str'

echo '$ trie --node-type int'
trie --node-type int 2>&1 ||
echo 'command failed: trie --node-type int'

echo '$ trie --node-type float'
trie --node-type float 2>&1 ||
echo 'command failed: trie --node-type float'

echo '$ trie --node-type foo'
trie --node-type foo 2>&1 ||
echo 'command failed: trie --node-type foo'

echo '$ trie --trie-type array --trie-type ternary'
trie --trie-type array --trie-type ternary 2>&1 ||
echo 'command failed: trie --trie-type array --trie-type ternary'

echo '$ trie --trie-type array'
trie --trie-type array 2>&1 ||
echo 'command failed: trie --trie-type array'

echo '$ trie --trie-type foo'
trie --trie-type foo 2>&1 ||
echo 'command failed: trie --trie-type foo'

echo '$ trie --output-type c-code --output-type object'
trie --output-type c-code --output-type object 2>&1 ||
echo 'command failed: trie --output-type c-code --output-type object'

echo '$ trie --output-type c-code --output-type obj'
trie --output-type c-code --output-type obj 2>&1 ||
echo 'command failed: trie --output-type c-code --output-type obj'

echo '$ trie --output-type c-code --output-type o'
trie --output-type c-code --output-type o 2>&1 ||
echo 'command failed: trie --output-type c-code --output-type o'

echo '$ trie --output-type c-code'
trie --output-type c-code 2>&1 ||
echo 'command failed: trie --output-type c-code'

echo '$ trie --output-type c'
trie --output-type c 2>&1 ||
echo 'command failed: trie --output-type c'

echo '$ trie --output-type foo'
trie --output-type foo 2>&1 ||
echo 'command failed: trie --output-type foo'

echo '$ trie --gen-type wide --gen-type compact'
trie --gen-type wide --gen-type compact 2>&1 ||
echo 'command failed: trie --gen-type wide --gen-type compact'

echo '$ trie --gen-type wide --gen-type comp'
trie --gen-type wide --gen-type comp 2>&1 ||
echo 'command failed: trie --gen-type wide --gen-type comp'

echo '$ trie --gen-type wide --gen-type c'
trie --gen-type wide --gen-type c 2>&1 ||
echo 'command failed: trie --gen-type wide --gen-type c'

echo '$ trie --gen-type wide'
trie --gen-type wide 2>&1 ||
echo 'command failed: trie --gen-type wide'

echo '$ trie --gen-type w'
trie --gen-type w 2>&1 ||
echo 'command failed: trie --gen-type w'

echo '$ trie --gen-type foo'
trie --gen-type foo 2>&1 ||
echo 'command failed: trie --gen-type foo'

echo '$ trie --debug'
trie --debug 2>&1 ||
echo 'command failed: trie --debug'

echo '$ trie --debug=0'
trie --debug=0 2>&1 ||
echo 'command failed: trie --debug=0'

echo '$ trie --debug=1'
trie --debug=1 2>&1 ||
echo 'command failed: trie --debug=1'

echo '$ trie --debug=2'
trie --debug=2 2>&1 ||
echo 'command failed: trie --debug=2'

echo '$ trie --debug=3'
trie --debug=3 2>&1 ||
echo 'command failed: trie --debug=3'

echo '$ trie --debug=4'
trie --debug=4 2>&1 ||
echo 'command failed: trie --debug=4'

echo '$ trie --debug=5'
trie --debug=5 2>&1 ||
echo 'command failed: trie --debug=5'

echo '$ trie --debug=6'
trie --debug=6 2>&1 ||
echo 'command failed: trie --debug=6'

echo '$ trie --debug=7'
trie --debug=7 2>&1 ||
echo 'command failed: trie --debug=7'

echo '$ trie --debug=8'
trie --debug=8 2>&1 ||
echo 'command failed: trie --debug=8'

echo '$ trie --debug=9'
trie --debug=9 2>&1 ||
echo 'command failed: trie --debug=9'

echo '$ trie --debug=10'
trie --debug=10 2>&1 ||
echo 'command failed: trie --debug=10'

echo '$ trie --debug=100'
trie --debug=100 2>&1 ||
echo 'command failed: trie --debug=100'

echo '$ trie --debug --no-debug'
trie --debug --no-debug 2>&1 ||
echo 'command failed: trie --debug --no-debug'

echo '$ trie --no-debug='
trie --no-debug= 2>&1 ||
echo 'command failed: trie --no-debug='

echo '$ trie --no-verbose'
trie --no-verbose 2>&1 ||
echo 'command failed: trie --no-verbose'

echo '$ trie --verbose'
trie --verbose 2>&1 ||
echo 'command failed: trie --verbose'

echo '$ trie --verbose --verbose'
trie --verbose --verbose 2>&1 ||
echo 'command failed: trie --verbose --verbose'

echo '$ trie --verbose --verbose --verbose'
trie --verbose --verbose --verbose 2>&1 ||
echo 'command failed: trie --verbose --verbose --verbose'

echo '$ trie --verbose --verbose --verbose --verbose'
trie --verbose --verbose --verbose --verbose 2>&1 ||
echo 'command failed: trie --verbose --verbose --verbose --verbose'

echo '$ trie --verbose --verbose --verbose --verbose --no-verbose'
trie --verbose --verbose --verbose --verbose --no-verbose 2>&1 ||
echo 'command failed: trie --verbose --verbose --verbose --verbose --no-verbose'

echo '$ trie -V'
trie -V 2>&1 ||
echo 'command failed: trie -V'

echo '$ trie -VV'
trie -VV 2>&1 ||
echo 'command failed: trie -VV'

echo '$ trie -VVV'
trie -VVV 2>&1 ||
echo 'command failed: trie -VVV'

echo '$ trie -VVVV'
trie -VVVV 2>&1 ||
echo 'command failed: trie -VVVV'

echo '$ trie -VVVVV'
trie -VVVVV 2>&1 ||
echo 'command failed: trie -VVVVV'
)
