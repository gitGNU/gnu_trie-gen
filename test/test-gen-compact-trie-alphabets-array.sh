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
# $ gen-test -T gen-compact-trie-alphabets-array
#

[[ "$1" =~ ^-u[0-9]+$ ]] &&
u="${1:2}" ||
u=""

diff -u$u -L gen-compact-trie-alphabets-array.old <(echo \
'$ print() { printf '\''%s\n'\'' "$@"; }
$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }
$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }
$ ../src/trie --attrs --dots --gen=compact --output=trie --trie=array < alphabets.txt
[25,'\''z'\''] {
.   "a" [0,"",'\''a'\'',[8,'\''z'\'']] {
.   .   "a" [0,"",'\''a'\'',[0]]: "aa"
.   .   "f" [0,"",'\''f'\'',[0]]: "af"
.   .   "gq" [0,"g",'\''q'\'',[0]]: "agq"
.   .   "k" [0,"",'\''k'\'',[0]]: "ak"
.   .   "m" [0,"",'\''m'\'',[0]]: "am"
.   .   "r" [0,"",'\''r'\'',[0]]: "ar"
.   .   "s" [0,"",'\''s'\'',[2,'\''t'\'']]: "as" {
.   .   .   "a" [0,"",'\''a'\'',[0]]: "asa"
.   .   .   "t" [0,"",'\''t'\'',[0]]: "ast"
.   .   }
.   .   "z" [0,"",'\''z'\'',[1,'\''_'\'']]: "az" {
.   .   .   "_Cyrl" [0,"_Cyr",'\''l'\'',[0]]: "az_Cyrl"
.   .   }
.   }
.   "b" [0,"",'\''b'\'',[9,'\''y'\'']] {
.   .   "as" [0,"a",'\''s'\'',[0]]: "bas"
.   .   "e" [0,"",'\''e'\'',[2,'\''z'\'']]: "be" {
.   .   .   "m" [0,"",'\''m'\'',[0]]: "bem"
.   .   .   "z" [0,"",'\''z'\'',[0]]: "bez"
.   .   }
.   .   "g" [0,"",'\''g'\'',[0]]: "bg"
.   .   "m" [0,"",'\''m'\'',[0]]: "bm"
.   .   "n" [0,"",'\''n'\'',[0]]: "bn"
.   .   "o" [0,"",'\''o'\'',[0]]: "bo"
.   .   "r" [0,"",'\''r'\'',[1,'\''x'\'']]: "br" {
.   .   .   "x" [0,"",'\''x'\'',[0]]: "brx"
.   .   }
.   .   "s" [0,"",'\''s'\'',[1,'\''_'\'']]: "bs" {
.   .   .   "_Cyrl" [0,"_Cyr",'\''l'\'',[0]]: "bs_Cyrl"
.   .   }
.   .   "yn" [0,"y",'\''n'\'',[0]]: "byn"
.   }
.   "c" [0,"",'\''c'\'',[5,'\''y'\'']] {
.   .   "a" [0,"",'\''a'\'',[0]]: "ca"
.   .   "gg" [0,"g",'\''g'\'',[0]]: "cgg"
.   .   "hr" [0,"h",'\''r'\'',[0]]: "chr"
.   .   "s" [0,"",'\''s'\'',[0]]: "cs"
.   .   "y" [0,"",'\''y'\'',[0]]: "cy"
.   }
.   "d" [0,"",'\''d'\'',[6,'\''z'\'']] {
.   .   "a" [0,"",'\''a'\'',[1,'\''v'\'']]: "da" {
.   .   .   "v" [0,"",'\''v'\'',[0]]: "dav"
.   .   }
.   .   "e" [0,"",'\''e'\'',[0]]: "de"
.   .   "je" [0,"j",'\''e'\'',[0]]: "dje"
.   .   "ua" [0,"u",'\''a'\'',[0]]: "dua"
.   .   "yo" [0,"y",'\''o'\'',[0]]: "dyo"
.   .   "z" [0,"",'\''z'\'',[0]]: "dz"
.   }
.   "e" [0,"",'\''e'\'',[9,'\''w'\'']] {
.   .   "bu" [0,"b",'\''u'\'',[0]]: "ebu"
.   .   "e" [0,"",'\''e'\'',[0]]: "ee"
.   .   "l" [0,"",'\''l'\'',[0]]: "el"
.   .   "n" [0,"",'\''n'\'',[0]]: "en"
.   .   "o" [0,"",'\''o'\'',[0]]: "eo"
.   .   "s" [0,"",'\''s'\'',[0]]: "es"
.   .   "t" [0,"",'\''t'\'',[0]]: "et"
.   .   "u" [0,"",'\''u'\'',[0]]: "eu"
.   .   "wo" [0,"w",'\''o'\'',[0]]: "ewo"
.   }
.   "f" [0,"",'\''f'\'',[7,'\''y'\'']] {
.   .   "a" [0,"",'\''a'\'',[0]]: "fa"
.   .   "f" [0,"",'\''f'\'',[0]]: "ff"
.   .   "i" [0,"",'\''i'\'',[1,'\''l'\'']]: "fi" {
.   .   .   "l" [0,"",'\''l'\'',[0]]: "fil"
.   .   }
.   .   "o" [0,"",'\''o'\'',[0]]: "fo"
.   .   "r" [0,"",'\''r'\'',[0]]: "fr"
.   .   "ur" [0,"u",'\''r'\'',[0]]: "fur"
.   .   "y" [0,"",'\''y'\'',[0]]: "fy"
.   }
.   "g" [0,"",'\''g'\'',[6,'\''v'\'']] {
.   .   "a" [0,"",'\''a'\'',[0]]: "ga"
.   .   "d" [0,"",'\''d'\'',[0]]: "gd"
.   .   "l" [0,"",'\''l'\'',[0]]: "gl"
.   .   "sw" [0,"s",'\''w'\'',[0]]: "gsw"
.   .   "u" [0,"",'\''u'\'',[1,'\''z'\'']]: "gu" {
.   .   .   "z" [0,"",'\''z'\'',[0]]: "guz"
.   .   }
.   .   "v" [0,"",'\''v'\'',[0]]: "gv"
.   }
.   "h" [0,"",'\''h'\'',[6,'\''y'\'']] {
.   .   "a" [0,"",'\''a'\'',[1,'\''w'\'']]: "ha" {
.   .   .   "w" [0,"",'\''w'\'',[0]]: "haw"
.   .   }
.   .   "e" [0,"",'\''e'\'',[0]]: "he"
.   .   "i" [0,"",'\''i'\'',[0]]: "hi"
.   .   "r" [0,"",'\''r'\'',[0]]: "hr"
.   .   "u" [0,"",'\''u'\'',[0]]: "hu"
.   .   "y" [0,"",'\''y'\'',[0]]: "hy"
.   }
.   "i" [0,"",'\''i'\'',[6,'\''t'\'']] {
.   .   "a" [0,"",'\''a'\'',[0]]: "ia"
.   .   "d" [0,"",'\''d'\'',[0]]: "id"
.   .   "g" [0,"",'\''g'\'',[0]]: "ig"
.   .   "i" [0,"",'\''i'\'',[0]]: "ii"
.   .   "s" [0,"",'\''s'\'',[0]]: "is"
.   .   "t" [0,"",'\''t'\'',[0]]: "it"
.   }
.   "j" [0,"",'\''j'\'',[3,'\''m'\'']] {
.   .   "a" [0,"",'\''a'\'',[0]]: "ja"
.   .   "go" [0,"g",'\''o'\'',[0]]: "jgo"
.   .   "mc" [0,"m",'\''c'\'',[0]]: "jmc"
.   }
.   "k" [0,"",'\''k'\'',[13,'\''y'\'']] {
.   .   "a" [0,"",'\''a'\'',[2,'\''m'\'']]: "ka" {
.   .   .   "b" [0,"",'\''b'\'',[0]]: "kab"
.   .   .   "m" [0,"",'\''m'\'',[0]]: "kam"
.   .   }
.   .   "de" [0,"d",'\''e'\'',[0]]: "kde"
.   .   "ea" [0,"e",'\''a'\'',[0]]: "kea"
.   .   "hq" [0,"h",'\''q'\'',[0]]: "khq"
.   .   "i" [0,"",'\''i'\'',[0]]: "ki"
.   .   "k" [0,"",'\''k'\'',[1,'\''j'\'']]: "kk" {
.   .   .   "j" [0,"",'\''j'\'',[0]]: "kkj"
.   .   }
.   .   "l" [0,"",'\''l'\'',[1,'\''n'\'']]: "kl" {
.   .   .   "n" [0,"",'\''n'\'',[0]]: "kln"
.   .   }
.   .   "m" [0,"",'\''m'\'',[0]]: "km"
.   .   "n" [0,"",'\''n'\'',[0]]: "kn"
.   .   "o" [0,"",'\''o'\'',[1,'\''k'\'']]: "ko" {
.   .   .   "k" [0,"",'\''k'\'',[0]]: "kok"
.   .   }
.   .   "s" [0,"",'\''s'\'',[3,'\''h'\'']] {
.   .   .   "b" [0,"",'\''b'\'',[0]]: "ksb"
.   .   .   "f" [0,"",'\''f'\'',[0]]: "ksf"
.   .   .   "h" [0,"",'\''h'\'',[0]]: "ksh"
.   .   }
.   .   "w" [0,"",'\''w'\'',[0]]: "kw"
.   .   "y" [0,"",'\''y'\'',[0]]: "ky"
.   }
.   "l" [0,"",'\''l'\'',[8,'\''v'\'']] {
.   .   "ag" [0,"a",'\''g'\'',[0]]: "lag"
.   .   "g" [0,"",'\''g'\'',[0]]: "lg"
.   .   "kt" [0,"k",'\''t'\'',[0]]: "lkt"
.   .   "n" [0,"",'\''n'\'',[0]]: "ln"
.   .   "o" [0,"",'\''o'\'',[0]]: "lo"
.   .   "t" [0,"",'\''t'\'',[0]]: "lt"
.   .   "u" [0,"",'\''u'\'',[2,'\''y'\'']]: "lu" {
.   .   .   "o" [0,"",'\''o'\'',[0]]: "luo"
.   .   .   "y" [0,"",'\''y'\'',[0]]: "luy"
.   .   }
.   .   "v" [0,"",'\''v'\'',[0]]: "lv"
.   }
.   "m" [0,"",'\''m'\'',[12,'\''y'\'']] {
.   .   "as" [0,"a",'\''s'\'',[0]]: "mas"
.   .   "er" [0,"e",'\''r'\'',[0]]: "mer"
.   .   "fe" [0,"f",'\''e'\'',[0]]: "mfe"
.   .   "g" [0,"",'\''g'\'',[2,'\''o'\'']]: "mg" {
.   .   .   "h" [0,"",'\''h'\'',[0]]: "mgh"
.   .   .   "o" [0,"",'\''o'\'',[0]]: "mgo"
.   .   }
.   .   "k" [0,"",'\''k'\'',[0]]: "mk"
.   .   "l" [0,"",'\''l'\'',[0]]: "ml"
.   .   "n" [0,"",'\''n'\'',[0]]: "mn"
.   .   "r" [0,"",'\''r'\'',[0]]: "mr"
.   .   "s" [0,"",'\''s'\'',[0]]: "ms"
.   .   "t" [0,"",'\''t'\'',[0]]: "mt"
.   .   "ua" [0,"u",'\''a'\'',[0]]: "mua"
.   .   "y" [0,"",'\''y'\'',[0]]: "my"
.   }
.   "n" [0,"",'\''n'\'',[11,'\''y'\'']] {
.   .   "aq" [0,"a",'\''q'\'',[0]]: "naq"
.   .   "b" [0,"",'\''b'\'',[0]]: "nb"
.   .   "d" [0,"",'\''d'\'',[0]]: "nd"
.   .   "e" [0,"",'\''e'\'',[0]]: "ne"
.   .   "l" [0,"",'\''l'\'',[0]]: "nl"
.   .   "mg" [0,"m",'\''g'\'',[0]]: "nmg"
.   .   "n" [0,"",'\''n'\'',[1,'\''h'\'']]: "nn" {
.   .   .   "h" [0,"",'\''h'\'',[0]]: "nnh"
.   .   }
.   .   "r" [0,"",'\''r'\'',[0]]: "nr"
.   .   "so" [0,"s",'\''o'\'',[0]]: "nso"
.   .   "us" [0,"u",'\''s'\'',[0]]: "nus"
.   .   "yn" [0,"y",'\''n'\'',[0]]: "nyn"
.   }
.   "o" [0,"",'\''o'\'',[3,'\''s'\'']] {
.   .   "m" [0,"",'\''m'\'',[0]]: "om"
.   .   "r" [0,"",'\''r'\'',[0]]: "or"
.   .   "s" [0,"",'\''s'\'',[0]]: "os"
.   }
.   "p" [0,"",'\''p'\'',[4,'\''t'\'']] {
.   .   "a" [0,"",'\''a'\'',[1,'\''_'\'']]: "pa" {
.   .   .   "_Arab" [0,"_Ara",'\''b'\'',[0]]: "pa_Arab"
.   .   }
.   .   "l" [0,"",'\''l'\'',[0]]: "pl"
.   .   "s" [0,"",'\''s'\'',[0]]: "ps"
.   .   "t" [0,"",'\''t'\'',[0]]: "pt"
.   }
.   "r" [0,"",'\''r'\'',[5,'\''w'\'']] {
.   .   "m" [0,"",'\''m'\'',[0]]: "rm"
.   .   "n" [0,"",'\''n'\'',[0]]: "rn"
.   .   "o" [0,"",'\''o'\'',[1,'\''f'\'']]: "ro" {
.   .   .   "f" [0,"",'\''f'\'',[0]]: "rof"
.   .   }
.   .   "u" [0,"",'\''u'\'',[0]]: "ru"
.   .   "w" [0,"",'\''w'\'',[1,'\''k'\'']]: "rw" {
.   .   .   "k" [0,"",'\''k'\'',[0]]: "rwk"
.   .   }
.   }
.   "s" [0,"",'\''s'\'',[16,'\''w'\'']] {
.   .   "a" [0,"",'\''a'\'',[2,'\''q'\'']] {
.   .   .   "h" [0,"",'\''h'\'',[0]]: "sah"
.   .   .   "q" [0,"",'\''q'\'',[0]]: "saq"
.   .   }
.   .   "bp" [0,"b",'\''p'\'',[0]]: "sbp"
.   .   "e" [0,"",'\''e'\'',[2,'\''s'\'']]: "se" {
.   .   .   "h" [0,"",'\''h'\'',[0]]: "seh"
.   .   .   "s" [0,"",'\''s'\'',[0]]: "ses"
.   .   }
.   .   "g" [0,"",'\''g'\'',[0]]: "sg"
.   .   "hi" [0,"h",'\''i'\'',[1,'\''_'\'']]: "shi" {
.   .   .   "_Latn" [1,"h_Lat",'\''n'\'',[0]]: "shi_Latn"
.   .   }
.   .   "i" [0,"",'\''i'\'',[0]]: "si"
.   .   "k" [0,"",'\''k'\'',[0]]: "sk"
.   .   "l" [0,"",'\''l'\'',[0]]: "sl"
.   .   "n" [0,"",'\''n'\'',[0]]: "sn"
.   .   "o" [0,"",'\''o'\'',[0]]: "so"
.   .   "q" [0,"",'\''q'\'',[0]]: "sq"
.   .   "r" [0,"",'\''r'\'',[1,'\''_'\'']]: "sr" {
.   .   .   "_Latn" [0,"_Lat",'\''n'\'',[0]]: "sr_Latn"
.   .   }
.   .   "s" [0,"",'\''s'\'',[1,'\''y'\'']]: "ss" {
.   .   .   "y" [0,"",'\''y'\'',[0]]: "ssy"
.   .   }
.   .   "t" [0,"",'\''t'\'',[0]]: "st"
.   .   "v" [0,"",'\''v'\'',[0]]: "sv"
.   .   "w" [0,"",'\''w'\'',[1,'\''c'\'']]: "sw" {
.   .   .   "c" [0,"",'\''c'\'',[0]]: "swc"
.   .   }
.   }
.   "t" [0,"",'\''t'\'',[11,'\''z'\'']] {
.   .   "a" [0,"",'\''a'\'',[0]]: "ta"
.   .   "e" [0,"",'\''e'\'',[1,'\''o'\'']]: "te" {
.   .   .   "o" [0,"",'\''o'\'',[0]]: "teo"
.   .   }
.   .   "g" [0,"",'\''g'\'',[0]]: "tg"
.   .   "h" [0,"",'\''h'\'',[0]]: "th"
.   .   "i" [0,"",'\''i'\'',[2,'\''g'\'']]: "ti" {
.   .   .   "_ER" [0,"_E",'\''R'\'',[0]]: "ti_ER"
.   .   .   "g" [0,"",'\''g'\'',[0]]: "tig"
.   .   }
.   .   "n" [0,"",'\''n'\'',[0]]: "tn"
.   .   "o" [0,"",'\''o'\'',[0]]: "to"
.   .   "r" [0,"",'\''r'\'',[0]]: "tr"
.   .   "s" [0,"",'\''s'\'',[0]]: "ts"
.   .   "wq" [0,"w",'\''q'\'',[0]]: "twq"
.   .   "zm" [0,"z",'\''m'\'',[0]]: "tzm"
.   }
.   "u" [0,"",'\''u'\'',[4,'\''z'\'']] {
.   .   "g" [0,"",'\''g'\'',[0]]: "ug"
.   .   "k" [0,"",'\''k'\'',[0]]: "uk"
.   .   "r" [0,"",'\''r'\'',[0]]: "ur"
.   .   "z" [0,"",'\''z'\'',[1,'\''_'\'']]: "uz" {
.   .   .   "_" [0,"",'\''_'\'',[2,'\''C'\'']] {
.   .   .   .   "Arab" [0,"Ara",'\''b'\'',[0]]: "uz_Arab"
.   .   .   .   "Cyrl" [0,"Cyr",'\''l'\'',[0]]: "uz_Cyrl"
.   .   .   }
.   .   }
.   }
.   "v" [0,"",'\''v'\'',[5,'\''u'\'']] {
.   .   "ai_Latn" [0,"ai_Lat",'\''n'\'',[0]]: "vai_Latn"
.   .   "e" [0,"",'\''e'\'',[0]]: "ve"
.   .   "i" [0,"",'\''i'\'',[0]]: "vi"
.   .   "o" [0,"",'\''o'\'',[0]]: "vo"
.   .   "un" [0,"u",'\''n'\'',[0]]: "vun"
.   }
.   "wa" [0,"w",'\''a'\'',[2,'\''l'\'']] {
.   .   "e" [1,"w",'\''e'\'',[0]]: "wae"
.   .   "l" [1,"w",'\''l'\'',[0]]: "wal"
.   }
.   "x" [0,"",'\''x'\'',[2,'\''o'\'']] {
.   .   "h" [0,"",'\''h'\'',[0]]: "xh"
.   .   "og" [0,"o",'\''g'\'',[0]]: "xog"
.   }
.   "y" [0,"",'\''y'\'',[2,'\''o'\'']] {
.   .   "av" [0,"a",'\''v'\'',[0]]: "yav"
.   .   "o" [0,"",'\''o'\'',[1,'\''_'\'']]: "yo" {
.   .   .   "_BJ" [0,"_B",'\''J'\'',[0]]: "yo_BJ"
.   .   }
.   }
.   "z" [0,"",'\''z'\'',[3,'\''u'\'']] {
.   .   "gh" [0,"g",'\''h'\'',[0]]: "zgh"
.   .   "h" [0,"",'\''h'\'',[1,'\''_'\'']]: "zh" {
.   .   .   "_Hant" [0,"_Han",'\''t'\'',[0]]: "zh_Hant"
.   .   }
.   .   "u" [0,"",'\''u'\'',[0]]: "zu"
.   }
}'
) -L gen-compact-trie-alphabets-array.new <(
echo '$ print() { printf '\''%s\n'\'' "$@"; }'
print() { printf '%s\n' "$@"; } 2>&1 ||
echo 'command failed: print() { printf '\''%s\n'\'' "$@"; }'

echo '$ printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'
printc() { awk '{ printf("%c\n", strtonum("0x" $1)) }' <<< "$1"; } 2>&1 ||
echo 'command failed: printc() { awk '\''{ printf("%c\n", strtonum("0x" $1)) }'\'' <<< "$1"; }'

echo '$ trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'
trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r 's|^#|\\\0|'; } 2>&1 ||
echo 'command failed: trie0() { set -o pipefail && ../src/trie "$@" 2>&1|sed -r '\''s|^#|\\\0|'\''; }'

echo '$ ../src/trie --attrs --dots --gen=compact --output=trie --trie=array < alphabets.txt'
../src/trie --attrs --dots --gen=compact --output=trie --trie=array < alphabets.txt 2>&1 ||
echo 'command failed: ../src/trie --attrs --dots --gen=compact --output=trie --trie=array < alphabets.txt'
)

