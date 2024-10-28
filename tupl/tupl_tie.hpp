/*
 SPDX-FileCopyrightText: 2024 The Lemuriad <wjwray@gmail.com>
 SPDX-License-Identifier: BSL-1.0
 Repo: https://github.com/Lemuriad/tupl
*/
#ifndef LML_TUPL_TIE_HPP
#define LML_TUPL_TIE_HPP

#include "tupl.hpp"
#include "tupl_platform.hpp"

#include "namespace.hpp"

/*
  ties<E...> : tupl<E...>
  ==========
  'ties' is a tupl-derived type for performing 'collective assignments'
  through a tupl-of-references via a set of custom operator= overloads
  designed to mimic 'aggregate assignment' of tupls of values.

  lml::ties is used as a temporary 'assignment wrapper' on the LHS of
  assignment operations from a tupl or braced-list of values on the RHS.

  lml::ties is not trivially copyable but remains an aggregate type.
  While ties can be stored in arrays and used in algorithms, reference-
  semantics is problematic so such advanced usage is not recommended.

  lml::tie(a,b) -> lml::ties<A&,B&>{a,b}
  =============
  The lml::tie function returns lml::ties of references to its arguments
  and is used on the LHS of assignments as follows:

    int a;
    std::string b, c="two";
    lml::tie(a,b) = {};
    lml::tie(a,b) = {1,"one"};
    lml::tie(a,b) = {2,std::move(c)};

  This enforces transient lifetime following an idiom set by std::tie.

  getie<I...>(tuplish) -> lml::ties{get<I>(tuplish)...}
  ====================
  The getie function returns a ties-tupl of references to explicitly
  indexed elements of its tuplish argument.

  Comparison to std::tie
  ======================
  lml::tie accepts braced-list RHS to mimic tupl aggregate assignment.
  std::tie does not accept braced-list RHS.

  lml::ties supports C-array elements, like lml::tupl.
  std::tuple does not support C-array elements.

  lml::ties assignments return void, to prevent chained assignments.
  std::tuple assignments return *this.

  lml::ties are only same-type comparable (c.f. lml::cmps).
  std::tuple supports heterogeneous comparisons.

  std <tuple> has no equivalent of getie<I...>(tuple)
*/

// lml::ties intentionally leaves the copy constructor implicit in order
// to remain an aggregate type.
// Clang warns about this by default so the warning must be silenced.
NO_WARN_DEPRECATED_COPY()
// warning: definition of implicit copy constructor for ties<E...> is
// deprecated because it has a user-declared copy assignment operator
// [-Wdeprecated-copy]

/******************************************************************
  ties assignment operator= overloads
  ===================================

  Six operator=(rhs) overloads help mimic tuple-of-value assignments.
  All apart from 1a are const qualified and require tupl_tie<ties>

  1a operator=(ties) =default  copy-assign for all-value elements
  1b operator=(ties) const     copy-assign for all-reference elements

  2  operator=(tuplish) const  move or copy-assign from other tuplish
                               based on ref quals of RHS types

  3a operator=({rvals}) const  move-assigns from init-list of rvalues
  3b operator=({lvals}) const  copy-assigns from init-list of lvalues

  4  operator=({}) const       assign from empty init list
                               i.e. 'clear' all elements

  Only overload 2 can perform mixed copy or move assignments but it
  cannot accept braced init-list RHS (because it deduces its type).
  Overloads 3a & 3b perform all move or all copy from braced init-lists
  with moves preferred if all initializers are rvalues.
*/

template <typename...E> struct ties : tupl<E...>
{
  // copy-assign for value tuples default delegates to base operator=
  ties& operator=(ties const& r) requires tupl_val<ties> = default;

  // copy-assign ties = ties, replaces deleted default assign
  constexpr void operator=(ties const& r) const
    noexcept(noexcept(assign_from(r)))
    requires tupl_tie<ties>
  { assign_from(r); }

  // assign ties = tuplish, allows non-narrowing conversions only
  template <tuplish T>
  constexpr void operator=(T&& r) const
    noexcept(noexcept(assign_from(r)))
    requires (tupl_tie<ties>
              && types_all<is_non_narrow_const_assignable,ties,tupl_t<T>>)
  { assign_from(r); }

  // assign ties = tupl_move, from init-list of rvalues; move is better match
  template <typename...>
  constexpr void operator=(tupl_move_t<ties> r) const
    noexcept(noexcept(assign_from(r)))
    requires (tupl_tie<ties> && move_assignable<E...>)
  { assign_from(r); }

  // assign ties = tupl_view, from init-list of lvalues; copy is worse match
  template <typename A = tupl_view_t<ties>, typename B = A>
  constexpr void operator=(A const& r) const
    noexcept(noexcept(assign_from(r)))
    requires(std::same_as<A,B>&& tupl_tie<ties>&& copy_assignable<E...>)
  { assign_from(r); }

  // assign this = {}, assignment from empty list for each element
  constexpr void operator=(std::true_type) const
    noexcept((is_nothrow_empty_list_assignable<E>() && ...))
    requires (tupl_tie<ties> && (empty_list_assignable<E> && ...))
  {
    map(as_tupl_t(*this),
        [](auto&...t) noexcept((is_nothrow_empty_list_assignable<E>() && ...))
        {
          ((assign(t)={}),...);
        });
  }

  // ties::assign_from(tuplish)
  template <template<typename...>class Tupl, typename...U>
  constexpr void assign_from(Tupl<U...> const& r) const
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

END_NO_WARN_DEPRECATED_COPY()

// ties CTAD, deduces lvalue references
//
template <typename...E> ties(E&...) -> ties<E&...>;

// ties CTAD from tupl base class
//
template <typename...E> ties(tupl<E&...>) -> ties<E&...>;

//
// tie(a...) -> ties<decltype(a)&...>{a...}; rejects rvalue arguments
//
template <typename...T>
constexpr auto tie(T&...t) noexcept -> ties<T&...>
{
  return { t... };
}

//
// getie<I...>(tupl) -> tie(get<I>(tupl)...)
//
template <unsigned...I>
constexpr auto getie(tuplish auto&& t) noexcept
  -> ties<decltype(get<I>((decltype(t))t))...> const
     requires ((I < tupl_size_v<decltype(t)>) && ...)
{
  return {get<I>((decltype(t))t)...};
}

#include "namespace.hpp"

#include "tupl_platform.hpp"
#endif
