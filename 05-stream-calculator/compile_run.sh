#!/bin/sh

set -xe

. ./flags.env

gcc ${COMPILER_FLAGS} "$*" main.c -o main.out
./main.out
