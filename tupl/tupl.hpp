/*
 SPDX-FileCopyrightText: 2024 The Lemuriad <wjwray@gmail.com>
 SPDX-License-Identifier: BSL-1.0
 Repo: https://github.com/Lemuriad/tupl
*/
#ifndef LML_TUPL_HPP
#define LML_TUPL_HPP
/*
  tupl<T...>
  ==========
  tupl is a C++20 tuple type implemented as struct specializations
  for a variadic number of members T... from zero up to TUPL_MAX_ARITY
  (16 by default, or as configured by the build option tupl_max_arity).

    tupl<T...> a "rule-of-zero" struct aggregate tuple type.
    lupl<T...> like tupl but with standard layout compatibility.
    ties<T...> derives from tupl<T...>, adds operator= overloads
    vals<T...> derives from tupl<T...>, adds operator= overloads
    cmps<T...> derives from tupl<T...>, adds mixed type comparisons.

    fwds<T...>, lvals<T...>, rvals<T...> and cvals<T...>
      derive from tupl<T...>, add custom CTAD only (nothing else added)

  Depends on <concepts> for assignable_from, ranges::swap CPO, etc.
  Depends on <compare> for three-way operator <=> comparisons, etc.
  Depends on <cstdint> for uintptr_t etc.

  Depends on c_array_support project for generic support of array
             assignment and comparison, alongside other regular types.

  Depends on IREPEAT preprocessor library for repetitive codegen
             (an optional dependency, only needed to do codegen).
*/
#include "tupl_traits.hpp"   // #includes "c_array_assign.hpp", required
#include <c_array_compare.hpp> // C array comparison support is required

#include "tupl_platform.hpp"

#include "namespace.hpp"

// If tupl_impl/tupl_impl.hpp exists then is included instead.
//
#ifdef TUPL_IMPL_PREPROCESS // Force preprocessing pass
#include "tupl_impl/tupl_impl.pp"
#elif defined __has_include
# if __has_include("tupl_impl.hpp")
#include "tupl_impl.hpp" /* DO NOT EDIT the file 'tupl_impl.hpp' */
# endif
#else
#include "tupl_impl/tupl_impl.hpp"
#endif

#include "namespace.hpp"

#include "tupl_platform.hpp" // #undef platform macros UNREACHABLE, NUA

#endif
