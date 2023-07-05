#!/bin/sh

set -xe

. ./flags.env

# -Wno-unused-local-typedefs:
# to use compile-time assert inside funtions
gcc ${COMPILER_FLAGS} -Wno-unused-local-typedefs test.c -o test.out
./test.out
