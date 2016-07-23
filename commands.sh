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

usage()
{
    echo "usage: $1 [$(sed 's/^://;s/[^:]/|-\0/g;s/:/ <arg>/g;s/^|//' <<< "$2")]"
}

quote()
{
    local __n__
    local __v__

    [ -z "$1" -o "$1" == "__n__" -o "$1" == "__v__" ] &&
    return 1

    printf -v __n__ '%q' "$1"
    eval __v__="\"\$$__n__\""
    #!!! echo "!!! 0 __v__='$__v__'"
    test -z "$__v__" && return 0
    printf -v __v__ '%q' "$__v__"
    #!!! echo "!!! 1 __v__='$__v__'"
    printf -v __v__ '%q' "$__v__"  # double quote
    #!!! echo "!!! 2 __v__='$__v__'"
    test -z "$SHELL_BASH_QUOTE_TILDE" &&
    __v__="${__v__//\~/\\~}"
    eval "$__n__=$__v__"
}

assign2()
{
    local __n__
    local __v__

    [ -z "$1" -o "$1" == "__n__" -o "$1" == "__v__" ] && return 1
    [ -z "$2" -o "$2" == "__n__" -o "$2" == "__v__" ] && return 1
    printf -v __n__ '%q' "$2"
    eval __v__="\"\$$__n__\""
    test -n "$__v__" &&
    printf -v __v__ '%q' "$__v__"
    printf -v __n__ '%q' "$1"
    test -z "$SHELL_BASH_QUOTE_TILDE" &&
    __v__="${__v__//\~/\\~}"
    eval "$__n__=$__v__"
}

optopt()
{
    local __n__="${1:-$opt}"       #!!!NONLOCAL
    local __v__=''
    test -n "$__n__" &&
    printf -v __v__ '%q' "$__n__"  # paranoia
    test -z "$SHELL_BASH_QUOTE_TILDE" &&
    __v__="${__v__//\~/\\~}"
    eval "$__n__=$__v__"
}

optarg()
{
    local __n__="${1:-$opt}"       #!!!NONLOCAL
    local __v__=''
    test -n "$OPTARG" &&
    printf -v __v__ '%q' "$OPTARG" #!!!NONLOCAL
    test -z "$SHELL_BASH_QUOTE_TILDE" &&
    __v__="${__v__//\~/\\~}"
    eval "$__n__=$__v__"
}

optact()
{
    local __v__="${1:-$opt}"       #!!!NONLOCAL
    printf -v __v__ '%q' "$__v__"  # paranoia
    test -z "$SHELL_BASH_QUOTE_TILDE" &&
    __v__="${__v__//\~/\\~}"
    eval "act=$__v__"
}

optlong()
{
    local a="$1"
    if [ "$a" == '-' ]; then
        if [ -z "$OPT" ]; then                                      #!!!NONLOCAL
            local A="${OPTARG%%=*}"                                 #!!!NONLOCAL
            OPT="-$opt$A"                                           #!!!NONLOCAL
            OPTN="${OPTARG:$((${#A})):1}"                           #!!!NONLOCAL
            OPTARG="${OPTARG:$((${#A} + 1))}"                       #!!!NONLOCAL
        else
            OPT="--$OPT"                                            #!!!NONLOCAL
        fi
    elif [ "$opt" == '-' -o \( -n "$a" -a -z "$OPT" \) ]; then      #!!!NONLOCAL
        OPT="${OPTARG%%=*}"                                         #!!!NONLOCAL
        OPTN="${OPTARG:$((${#OPT})):1}"                             #!!!NONLOCAL
        OPTARG="${OPTARG:$((${#OPT} + 1))}"                         #!!!NONLOCAL
        [ -n "$a" ] && OPT="$a-$OPT"                                #!!!NONLOCAL
    elif [ -z "$a" ]; then                                          #!!!NONLOCAL
        OPT=''                                                      #!!!NONLOCAL
        OPTN=''                                                     #!!!NONLOCAL
    fi
}

optlongchkarg()
{
    test -z "$OPT" &&                               #!!!NONLOCAL
    return 0

    [[ "$opt" == [a-zA-Z] ]] || {                   #!!!NONLOCAL
        error "internal: invalid opt name '$opt'"   #!!!NONLOCAL
        return 1
    }

    local r="^:[^$opt]*$opt(.)"
    [[ "$opts" =~ $r ]]
    local m="$?"

    local s
    if [ "$m" -eq 0 ]; then
        s="${BASH_REMATCH[1]}"
    elif [ "$m" -eq 1 ]; then
        error "internal: opt '$opt' not in '$opts'" #!!!NONLOCAL
        return 1
    elif [ "$m" -eq "2" ]; then
        error "internal: invalid regex: $r"
        return 1
    else
        error "internal: unexpected regex match result: $m: ${BASH_REMATCH[@]}"
        return 1
    fi

    if [ "$s" == ':' ]; then
        test -z "$OPTN" && {                        #!!!NONLOCAL
            error --long -a
            return 1
        }
    else
        test -n "$OPTN" && {                        #!!!NONLOCAL
            error --long -d
            return 1
        }
    fi
    return 0
}

error()
{
    local __self__="$self"     #!!!NONLOCAL
    local __help__="$help"     #!!!NONLOCAL
    local __OPTARG__="$OPTARG" #!!!NONLOCAL
    local __opts__="$opts"     #!!!NONLOCAL
    local __opt__="$opt"       #!!!NONLOCAL
    local __OPT__="$OPT"       #!!!NONLOCAL

    local self="error"

    # actions: \
    #  a:argument for option -$OPTARG not found|
    #  o:when $OPTARG != '?': invalid command line option -$OPTARG, or, \
    #    otherwise, usage|
    #  i:invalid argument '$OPTARG' for option -$opt|
    #  d:option '$OPTARG' does not take arguments|
    #  e:error message|
    #  w:warning message|
    #  u:unexpected option -$opt|
    #  g:when $opt == ':': equivalent with 'a', \
    #    when $opt == '?': equivalent with 'o', \
    #    when $opt is anything else: equivalent with 'u'

    local act="e"
    local A="$__OPTARG__" # $OPTARG
    local h="$__help__"   # $help
    local m=""            # error msg
    local O="$__opts__"   # $opts
    local P="$__opt__"    # $opt
    local L="$__OPT__"    # $OPT
    local S="$__self__"   # $self

    local long=''         # short/long opts (default)

    #!!! echo "!!! A='$A'"
    #!!! echo "!!! O='$O'"
    #!!! echo "!!! P='$P'"
    #!!! echo "!!! L='$L'"
    #!!! echo "!!! S='$S'"

    local opt
    local opts=":aA:degh:iL:m:oO:P:S:uw-:"
    local OPTARG
    local OPTERR=0
    local OPTIND=1
    while getopts "$opts" opt; do
        case "$opt" in
            [adeiouwg])
                act="$opt"
                ;;
            #[])
            #	optopt
            #	;;
            [AhLmOPS])
                optarg
                ;;
            \:)	echo "$self: error: argument for option -$OPTARG not found" >&2
                return 1
                ;;
            \?)	if [ "$OPTARG" != "?" ]; then
                    echo "$self: error: invalid command line option -$OPTARG" >&2
                else
                    echo "$self: $(usage $self $opts)"
                fi
                return 1
                ;;
            -)	case "$OPTARG" in
                    long|long-opts)
                        long='l' ;;
                    short|short-opts)
                        long='' ;;
                    *)	echo "$self: error: invalid command line option --$OPTARG" >&2
                        return 1
                        ;;
                esac
                ;;
            *)	echo "$self: error: unexpected option -$OPTARG" >&2
                return 1
                ;;
        esac
    done
    #!!! echo "!!! A='$A'"
    #!!! echo "!!! O='$O'"
    #!!! echo "!!! P='$P'"
    #!!! echo "!!! L='$L'"
    #!!! echo "!!! S='$S'"
    shift $((OPTIND - 1))
    test -n "$1" && m="$1"
    local f="2"
    if [ "$act" == "g" ]; then
        if [ "$P" == ":" ]; then
            act="a"
        elif [ "$P" == "?" ]; then
            act="o"
        else 
            act="u"
        fi
    fi
    local o=''
    if [ -n "$long" -a -n "$L" ]; then
        test "${L:0:1}" != '-' && o+='--'
        o+="$L"
    elif [[ "$act" == [aod] ]]; then
        o="-$A"
    elif [[ "$act" == [iu] ]]; then
        o="-$P"
    fi
    case "$act" in
        a)	m="argument for option $o not found"
            ;;
        o)	if [ "$A" != "?" ]; then
                m="invalid command line option $o"
            else
                act="h"
                m="$(usage $S $O)"
                f="1"
            fi
            ;;
        i)	m="invalid argument for $o: '$A'"
            ;;
        u)	m="unexpected option $o"
            ;;
        d)	m="option $o does not take arguments"
            ;;
        *)	# [ew]
            if [ "$#" -ge "2" ]; then
                S="$1"
                m="$2"
            elif [ "$#" -ge "1" ]; then
                m="$1"
            fi
            ;;
    esac
    if [ "$act" == "w" ]; then
        m="warning${m:+: $m}"
    elif [ "$act" != "h" ]; then
        m="error${m:+: $m}"
    fi
    if [ -z "$S" -o "$S" == "-" ]; then
        printf "%s\n" "$m" >&$f
    else
        printf "%s: %s\n" "$S" "$m" >&$f
    fi
    if [ "$act" == "h" ]; then
        test -n "$1" && h="$1"
        test -n "$h" &&
        printf "%s\n" "$h" >&$f
    fi
    return $f
}

gen-func()
{
    local self="gen-func"
    local home="$HOME/trie-gen"
    local nrex='^(::)?[a-zA-Z_][a-zA-Z0-9_]*(::[a-zA-Z_][a-zA-Z0-9_]*)*$'
    local farg="func"
    local rarg="result"
    local fdef="lookup"
    local rdef="result_t"
    local tmpf="/tmp/trie-$self.XXX"

    local x="eval"
    local act="C"       # actions: \
                        #  C:gen code (default) (--gen-code)|
                        #  R:gen regex (--gen-regex)
    local f="+"         # function name (the default is 'lookup') (--func-name=STR)
    local h="+"         # home dir (the default is '$HOME/trie-gen') (--home=DIR)
    local i="-"         # input file (default is stdin) (--input=FILE)
    local r="+"         # result type name (the default is 'result_t') (--result-type=STR)
    local v=""          # be verbose (--verbose)

    local opt
    local OPT
    local OPTN
    local opts=":Cdf:h:i:r:Rvx-:"
    local OPTARG
    local OPTERR=0
    local OPTIND=1
    while getopts "$opts" opt; do
        # discriminate long options
        optlong

        # translate long options to short ones
        test -n "$OPT" &&
        case "$OPT" in
            gen-code)
                opt='C' ;;
            gen-regex)
                opt='R' ;;
            func-name|home|input|result-type|verbose)
                opt="${OPT:0:1}" ;;
            *)	error --long -o
                return 1
                ;;
        esac

        # check long option argument
        [[ "$opt" == [] ]] ||
        optlongchkarg ||
        return 1

        # handle short options
        case "$opt" in
            d)	x="echo"
                ;;
            x)	x="eval"
                ;;
            [CR])
                act="$opt"
                ;;
            [v])
                optopt
                ;;
            [hi])
                optarg
                ;;
            [fr])
                if [ -z "$OPTARG" ]; then
                    error  "argument for -${opt} cannot be null"
                    return 1
                elif [[ "$OPTARG" != "+" && ! ( "$OPTARG" =~ $nrex ) ]]; then
                    error --long -i
                    return 1;
                fi
                optarg
                ;;
            *)	error --long -g
                return 1
                ;;
        esac
    done
    shift $((OPTIND - 1))

    local n
    local v2
    for n in f r; do
        assign2 v2 $n        # used $f and $r
        [ "$v2" == "+" ] &&
        assign2 $n ${n}def   # used $f, $fdef, $r and $rdef
    done

    local h2
    local a
    local s
    local s2
    local s3
    local c
    if [ "$act" == "R" ]; then
        a='
            {
                # stev: TODO: quote perl regex chars of each $i
                s = NF > 1 ? $1 : ""
                for (i = 2; i < NF; i ++)
                    s = length(s) ? s "|" $i : $i
                if (substr($(NF), 1, 1) != "=") {
                    r = $(NF)
                    s = length(s) ? s "|" r : r
                }
                else
                    r = substr($(NF), 2)
                printf("\t\t\ts/^(\\t+)(\\t)return\\s*(\\x22(%s)\\x22);\\s*$/'
        [ -n "$v" ] &&
        a+='\\1\\2\\/\\/ input: \\3\\n'
        a+='\\1\\2t = '"$r"'::%s;\\n\\1\\2return true;\\n\\1}/\n", s, r)
            }'
        c="\
awk '$a'"
        $x "$c"
    elif [ "$act" == "C" ]; then
        test "$h" == "+" && h2="$home" || h2="$h"
        if [ -z "$h2" ]; then
            error "home dir cannot be null"
            return 1
        elif [ ! -d "$h2" ]; then
            error "home dir '$h2' not found"
            return 1
        fi
        local l="$h2/trie"
        if [ ! -x "$l" ]; then
            error "executable '$l' not found"
            return 1
        fi
        quote l

        ( # enter a subprocess
        local t="$tmpf"
        if [ "$x" == "eval" ]; then
            t="$(mktemp $t)" &&
            test -n "$t" || {
                error "inner command failed: mktemp"
                return 1
            }
            quote t
        fi
        trap 'rm -f $t' EXIT

        test "$h" != "+" && h2="$h" || h2=''
        quote h2

        test "$i" == "-" && i=''
        if [ -n "$i" -a ! -f "$i" ]; then
            error "input file '$i' not found"
            test "$x" == "eval" && return 1
        fi
        quote i
        quote r

        c="\
tee $t${i:+ < $i}|
$self ${h2:+-h $h2 }-${v}R -r $r"
        test "$x" == "echo" &&
        echo "$c"
        s2="$(eval "$c")" || {
            error "inner command failed [0]"
            return 1
        }

        a='
            {
                for (i = 1; i < NF; i ++)
                    printf("%s\n", $i)
                if (substr($(NF), 1, 1) != "=")
                    printf("%s\n", $(NF))
            }'
        c="\
awk '$a' \\
$t|
$l -T ternary -oe -ec -gc"
        test "$x" == "echo" &&
        echo "$c"
        s3="$(eval "$c")" || {
            error "inner command failed [1]"
            return 1
        }

        s='
            1i\
bool '"$f"'(const char* n, '"$r"'& t)\
{\
    // pattern: '"$s3"'
            $a}
            :0
            s/^(\t*)    /\1\t/
            t0
            s/(\*\s*)p\b/\1n/g'"${s2:+
$s2}"'
            s/\*n\s*==\s*0\s*\)$/\0 {/
            s/(return\s+)error\b/\1false/'
        c="\
awk '$a' \\
$t|
$l -T ternary -oc -gc|
sed -r '$s'|
sed -r ':0;s/^( *)\t/\1    /;t0'"

        $x "$c"
        ) # leave the subprocess
    else
        error "internal: unexpected act='$act'"
        return 1
    fi
}

