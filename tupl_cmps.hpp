//  Copyright (c) 2022 Lemurian Labs https://lemurianlabs.com/
//
//  Distributed under the Boost Software License, Version 1.0.
//        http://www.boost.org/LICENSE_1_0.txt
//
//  Repo: https://github.com/Lemurian-Labs/tupl

#ifndef LML_TUPL_CMPS_HPP
#define LML_TUPL_CMPS_HPP

#include "tupl.hpp"

#include "namespace.hpp"

/*
  cmps<E...> derives from tupl<E...> and adds comparison operators

  1  operator<=>(tuplish); compares any
  2a operator=({rvals}); move-assigns from braced init-list of rvalues
  2b operator=({lvals}); copy-assigns from braced init-list of lvalues
*/
template <typename...E> struct cmps : tupl<E...>
{
  friend auto operator<=>(cmps const&,cmps const&) = default;
  friend bool operator==(cmps const&,cmps const&) = default;

  template <tuplish R>
  friend constexpr auto operator<=>(cmps const& l, R const& r)  noexcept
    requires types_all<is_three_way_comparable_with,cmps,tupl_t<R>>
  {
    return compare3way(l,r);
  }
  template <tuplish R>
  friend constexpr bool operator==(cmps const& l, R const& r) noexcept
    requires types_all<is_equality_comparable_with,cmps,tupl_t<R>>
  {
    return equals(l,r);
  }
  template <tuplish R>
  friend constexpr bool operator==(cmps<>, R const& r) noexcept
    requires (sizeof...(E) == 0 && requires { equals(r,{}); })
  {
    return equals(r,{});
  }
};
// cmps CTAD
//
template <typename...E> cmps(E const&...) -> cmps<tupl_view_t<E>...>;

#include "namespace.hpp"

#endif
