#!/bin/sh
set -eu

HOST_CXX="${1:-c++}"
HOST_CXXFLAGS="${2:--std=c++98 -Wall -Wextra -Werror -ILEMBALL -ILEMBALL/VISOS}"

SRC_FILES=$(find LEMBALL -type f \( -name '*.cpp' -o -name '*.CPP' \) | sort)

if [ -z "${SRC_FILES}" ]; then
    echo "no source files found under LEMBALL/"
    exit 0
fi

for src in ${SRC_FILES}; do
    "${HOST_CXX}" ${HOST_CXXFLAGS} -fsyntax-only -x c++ "${src}"
done

echo "host C++ syntax check passed"
