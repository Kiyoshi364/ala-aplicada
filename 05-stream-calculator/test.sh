#!/bin/sh

set -xe

. ./flags.env

gcc ${COMPILER_FLAGS} test.c -o test.out
./test.out
