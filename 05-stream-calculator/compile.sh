#!/bin/sh

set -xe

COMPILER_WARNS="-Warith-conversion -Wcast-align=strict -Wcast-qual -Wshadow -Wstrict-prototypes -Wswitch-default -Wundef -Wall -Wextra -Wstrict-overflow=5"
# COMPILER_PEDANTIC="-Wattributes -Wimplicit-int -Wimplicit-function-declaration -Wincompatible-pointer-types -Wint-conversion -Wmain -Wpointer-arith -Wpointer-sign -Wincompatible-pointer-types -Wwrite-strings"
COMPILER_PEDANTIC="-Wpedantic"
COMPILER_FLAGS="-std=c11 ${COMPILER_PEDANTIC} ${COMPILER_WARNS} -Werror"

gcc ${COMPILER_FLAGS} main.c -o main
