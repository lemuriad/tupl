/*
 SPDX-FileCopyrightText: 2024 The Lemuriad <wjwray@gmail.com>
 SPDX-License-Identifier: BSL-1.0
 Repo: https://github.com/Lemuriad/tupl
*/
#ifndef LML_TUPL_VALS_HPP
#define LML_TUPL_VALS_HPP

#include "tupl.hpp"

#include "namespace.hpp"

/*
  vals<E...> derives from tupl<E...> and adds operator= overloads

  1  operator=(tuplish); Non-narrowing copy-conversions from other tupl
  2a operator=({rvals}); move-assigns from braced init-list of rvalues
  2b operator=({lvals}); copy-assigns from braced init-list of lvalues
*/
template <typename...E> struct vals : tupl<E...>
{
  // assign vals = tuplish, allowing only non-narrowing conversions
  template <tuplish T>
  constexpr auto& operator=(T&& r)
    noexcept(noexcept(assign_to{*this} = (T&&)r))
    requires (types_all<is_non_narrow_assignable,vals,tupl_t<T>>)
  { return assign_to{*this} = (T&&)r; }

  // assign vals = tupl_move rvalue init list; move is better match
  template <typename...>
  constexpr auto& operator=(tupl_move_t<vals> r)
    noexcept(noexcept(assign_to{*this} = (decltype(r)&&)r))
    requires move_assignable<E...>
  { return assign_to{*this} = (decltype(r)&&)r; }

  // assign tupl_val = tupl_view lvalue init list; copy is worse match
  template <typename A = tupl_view_t<vals>, typename B = A>
  constexpr auto& operator=(B r)
    noexcept(noexcept(assign_to{*this} = r))
    requires (std::same_as<A,B> && copy_assignable<E...>)
  { return assign_to{*this} = r; }
};
// vals CTAD
//
template <typename...E> vals(E const&...) -> vals<E...>;

#include "namespace.hpp"

#endif
