#!/usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail
if [[ "${TRACE-0}" == "1" ]]; then
    set -o xtrace
fi

if [[ "${1-}" =~ ^-*h(elp)?$ ]]; then
    echo 'Usage: ./amalgamate.sh

This bash script generates single header include "tupl_amalgam.hpp".
'
    exit
fi

echo "/*
 SPDX-FileCopyrightText: 2023 The Lemuriad <opensource@lemurianlabs.com>
 SPDX-License-Identifier: BSL-1.0
 Repo: https://github.com/Lemuriad/tupl

  tupl is a C++20 type implemented as a rule-of-zero aggregate struct.
  Tested on GCC, Clang and recent MSVC.

  tupl_amalgam.hpp is a single header amalgamated from tupl #include's

  WARNING: Do not edit this file \"tupl_amalgam.hpp\"
  =======
* It's generated by preprocessing \"tupl_amalgam.pp\" and is overwritten
* whenever the preprocessing step is rerun.
***********************************************************************/
#ifndef LML_TUPL_HPP
#define LML_TUPL_HPP

#include <compare>
#include <concepts>
#include <cstdint>
#define UINTPTR uintptr_t
" > tupl_amalgam.hpp

sed -n /UNREACHABLE:/,/UNDEFINE/p ../tupl/tupl_platform.hpp | sed \$d >> tupl_amalgam.hpp

cat ../tupl_impl/namespace.hpp >> tupl_amalgam.hpp

sed -n '/^#include \"namespace.hpp\"/,//p' ../subprojects/c_array_support/util_traits.hpp | sed '1d;$d' >> tupl_amalgam.hpp

sed -n '/^#include \"namespace.hpp\"/,//p' ../subprojects/c_array_support/c_array_support.hpp | sed '/^#include/d' >> tupl_amalgam.hpp

sed -n '/^#include \"namespace.hpp\"/,//p' ../subprojects/c_array_support/c_array_compare.hpp | sed '1d;$d' >> tupl_amalgam.hpp

sed -n '/^#include \"namespace.hpp\"/,//p' ../subprojects/c_array_support/c_array_assign.hpp | sed '1d;$d' >> tupl_amalgam.hpp

sed -n '/^#include \"namespace.hpp\"/,//p' ../tupl/tupl_traits.hpp | sed '1d;$d' >> tupl_amalgam.hpp

sed -n '/\/\/$/',\$p $1 >> tupl_amalgam.hpp

sed -n '/^#include \"namespace.hpp\"/,//p' ../tupl/tupl_tie.hpp | sed '1d;$d' >> tupl_amalgam.hpp

sed -n '/^#include \"namespace.hpp\"/,//p' ../tupl/tupl_vals.hpp | sed '1d;$d' >> tupl_amalgam.hpp

sed -n '/^#include \"namespace.hpp\"/,//p' ../tupl/tupl_cmps.hpp | sed '1d;$d' >> tupl_amalgam.hpp

cat ../tupl_impl/namespace.hpp >> tupl_amalgam.hpp

sed -n /UNDEFINE/,\$p ../tupl/tupl_platform.hpp | sed 1d >> tupl_amalgam.hpp
