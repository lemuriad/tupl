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

echo "
//  Copyright (c) 2023 Lemurian Labs https://lemurianlabs.com/
//
//  Distributed under the Boost Software License, Version 1.0.
//        http://www.boost.org/LICENSE_1_0.txt
//
//  Repo: https://github.com/Lemurian-Labs/tupl
/*
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

sed -n 12,87p ../tupl_platform.hpp >> tupl_amalgam.hpp

cat ../tupl_dev/namespace.hpp >> tupl_amalgam.hpp

sed -n 15,106p ../subprojects/c_array_support/util_traits.hpp >> tupl_amalgam.hpp

sed -n '/^#include/d;63,275p' ../subprojects/c_array_support/c_array_support.hpp >> tupl_amalgam.hpp

sed -n 91,366p ../subprojects/c_array_support/array_compare.hpp >> tupl_amalgam.hpp

sed -n 97,275p ../subprojects/c_array_support/array_assign.hpp >> tupl_amalgam.hpp

sed -n 14,296p ../tupl_traits.hpp >> tupl_amalgam.hpp

sed -n 15,\$p $1 >> tupl_amalgam.hpp

sed -n 15,91p ../tupl_tie.hpp >> tupl_amalgam.hpp

sed -n 15,48p ../tupl_vals.hpp >> tupl_amalgam.hpp

sed -n 14,49p ../tupl_cmps.hpp >> tupl_amalgam.hpp

cat ../tupl_dev/namespace.hpp >> tupl_amalgam.hpp

sed -n 89,98p ../tupl_platform.hpp >> tupl_amalgam.hpp
