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

TARGETS := depend all clean allclean

.PHONY: default ${TARGETS}

default: all

GCC := gcc
GCC_VERSION := 40800
GCC_STD := gnu++11

CFLAGS := -Wall \
          -Wextra \
          -DDEBUG \
          -std=${GCC_STD} \
          -g

OBJS := $(patsubst %.cpp,%.o,${SRCS})

#
# GCC version checking
#

GCC_VERSION_CHECK = $(shell \
	bash -c '. ./gcc-version.sh && gcc-version-check ${GCC_VERSION} ${GCC}' 2>&1)

ifneq (${GCC_VERSION_CHECK},)
$(error ${GCC_VERSION_CHECK})
endif

#
# dependecy rules
#

ifeq (.depend, $(wildcard .depend))
include .depend
endif

${OBJS}: %.o: %.cpp

#
# main targets
#

all:: ${BINS}

clean::
	rm -f *.o

allclean:: clean
	rm -f *~ ${BINS}

#
# explicit building rules
#

depend::
	${GCC} ${CFLAGS} -c ${SRCS} -MM > .depend

${OBJS}:
	${GCC} ${CFLAGS} -c $< -o $@


