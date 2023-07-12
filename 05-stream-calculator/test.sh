#!/bin/sh

set -xe

. ./flags.env

compile_run () {
  # -Wno-unused-local-typedefs:
  # to use compile-time assert inside funtions
  compile="gcc ${COMPILER_FLAGS} -Wno-unused-local-typedefs"
  prog="tests/${1}.c"
  out="tests/${1}.out"
  ${compile} "${prog}" -o "${out}" && ./"${out}"
}
compile_run test_streams
