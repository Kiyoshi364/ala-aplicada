#!/bin/sh

set -x

. ./flags.env

total_errs=0
compile_run () {
  # -Wno-unused-local-typedefs:
  # to use compile-time assert inside funtions
  compile="gcc ${COMPILER_FLAGS} -Wno-unused-local-typedefs"
  prog="tests/${1}.c"
  out="tests/${1}.out"
  ${compile} "${prog}" -o "${out}" && ./"${out}"
  total_errs=$((${total_errs} + $?))
}
compile_run test_streams
compile_run test_lexer

set +x

if [ ${total_errs} -eq 0 ]; then
  echo "Summary: All ok!"
else
  echo "Summary: Found ${total_errs} in all tests!"
fi
