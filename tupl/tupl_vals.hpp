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
  vals<E...> : tupl<E...>
  ==========
  'vals' is a tupl-derived type that adds custom operator= overloads
  to deal with two assignability shortcomings of the base tupl:

    1. A tupl is assignable only from a same-type tupl on the RHS.
       Ability to assign from a tupl of forwarding references helps
       eliminate redundant copies implicit in 'aggregate assignment'.

    2. C array variables don't initialize or assign from array values
       so attempts to assign tupl C array elements from array values
       listed in a braced init-list RHS fails at the first step.

  Overload #1 provides 'heterogeneous' assignment from tuplish types
  of assignable-from element types, including references.

  Overloads #2a,2b handle 'braced init-list' assignments to C arrays.

  1  operator=(tuplish) move or copy-assign elements from other tuplish
                        based on the value category of RHS element types
                        allows non-narrowing conversions only.
                        RHS is deduced so cannot be a braced init-list.

  2a operator=({rvals}); move-assigns from braced init-list of rvalues
  2b operator=({lvals}); copy-assigns from braced init-list of lvalues
*/

template <typename...E> struct vals : tupl<E...>
{
  // assign vals = tuplish, allows non-narrowing conversions only
  template <tuplish T>
  constexpr void operator=(T&& r)
    noexcept(noexcept(assign_from(r)))
    requires (types_all<is_non_narrow_assignable,vals,tupl_t<T>>)
  { assign_from(r); }

  // assign vals = tupl_move, from init-list of rvalues; move is better match
  template <typename...>
  constexpr void operator=(tupl_move_t<vals> r)
    noexcept(noexcept(assign_from(r)))
    requires (is_move_assignable_v<E> && ...)
    { assign_from(r); }

  // assign vals = tupl_view, from init-list of lvalues; copy is worse match
  template <typename A = tupl_view_t<vals>, typename B = A>
  constexpr void operator=(B r)
    noexcept(noexcept(assign_from(r)))
    requires (std::same_as<A,B> // tie break against tupl_move_t
              && (is_copy_assignable_v<E> && ...))
  { assign_from(r); }

  // vals::assign_from(tuplish)
  template <template<typename...>class Tupl, typename...U>
  constexpr void assign_from(Tupl<U...> const& r)
    noexcept((is_nothrow_assignable_v<E&,U> && ...))
    requires (tuplish<Tupl<U...>> && (is_assignable_v<E&,U> && ...))
  {
    map(as_tupl_t(r),
        [this](auto&...u) noexcept((is_nothrow_assignable_v<E&,U> && ...))
        {
          assign_elements(*this, static_cast<U>(u)...);
        }
       );
  }
};

// vals CTAD, deduce all by-value including arrays with no decay
//
template <typename...E> vals(E const&...) -> vals<E...>;

#include "namespace.hpp"

#endif
