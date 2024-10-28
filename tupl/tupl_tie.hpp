/*
 SPDX-FileCopyrightText: 2024 The Lemuriad <wjwray@gmail.com>
 SPDX-License-Identifier: BSL-1.0
 Repo: https://github.com/Lemuriad/tupl
*/
#ifndef LML_TUPL_TIE_HPP
#define LML_TUPL_TIE_HPP

#include "tupl.hpp"

#include "namespace.hpp"

/*
  ties<E...> : tupl<E...>
  ==========
  'ties' is a tupl-derived type for performing 'collective assignments'.
  It is used as a reference-wrapper type, with reference semantics -
  it is not intended to be used as a value type with value semantics.

  It is used as a temporary, transient, 'assignment wrapper' on the LHS
  of '=' assignment ops from a tupl or braced-list of values on the RHS.

  Non-transient use is often problematic so is not recommended
  (language references prohibit array storage to avoid problems).

  lml::ties is not trivially copyable, but remains an aggregate type.

  lml::tie(a,b) -> lml::ties<A&,B&>{a,b}
  =============
  The tie function returns a ties-tupl of references to its arguments.
  Use it on the LHS of assignments:

    int a;
    std::string b, c="two";
    lml::tie(a,b) = {};
    lml::tie(a,b) = {1,"one"};
    lml::tie(a,b) = {2,std::move(c)};

  This enforces transient use following an idiom set by std::tie.

  Comparison to std::tie
  ======================
  The std <tuple> function std::tie does not accept braced list RHS.
  It returns a std::tuple of references for assign-through from a
  std::tuple of source values; std::tie(a,b) = std::tuple{2,"two"}.

  The ties type accepts braced list RHS in order to mimic the syntax
  that works for value tupls as aggregate types. However, assignment
  from braced lists only performs all-copy or all-move assignments.
  Mixed assignments require a tupl RHS of mixed lvalues and rvalues.

  std::tuple assignments return an lvalue reference to *this.
  lml::ties assignments return void to prevent chained assignments.

  std::tie supports all std::tuple heterogeneous comparisons.
  lml::ties are only same-type comparable (heterogeneous comparison
  is provided by the 'cmps' tupl-derived 'comparison wrapper' type).

  getie<I...>(tuplish) -> lml::ties{get<I>(tuplish)...}
  ====================
  The getie function returns a ties-tupl of references to explicitly
  indexed elements of its tuplish argument.


  *****************************************************************
  ties assignment operator= overloads
  ===================================

  Six operator=(rhs) overloads help mimic tuple-of-value assignments.
  All apart from 1a are const qualified and require tupl_tie<ties>

  1a operator=(ties)           copy-assign for tupl-of-values =default
  1b operator=(ties) const     copy-assign for tupl-of-refs

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

START_NO_WARN_DEPRECATED_COPY()

template <typename...E> struct ties : tupl<E...>
{
  // copy-assign for value tuples default delegates to base operator=
  ties& operator=(ties const& r) requires tupl_val<ties> = default;

  // copy-assign ties = ties replaces deleted default assign
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

  // assign ties = tupl_move rvalue init list; move is better match
  template <typename...>
  constexpr void operator=(tupl_move_t<ties> r) const
    noexcept(noexcept(assign_from(r)))
    requires (tupl_tie<ties> && move_assignable<E...>)
  { assign_from(r); }

  // assign ties = tupl_view lvalue init list; copy is worse match
  template <typename A = tupl_view_t<ties>, typename B = A>
  constexpr void operator=(A const& r) const
    noexcept(noexcept(assign_from(r)))
    requires(std::same_as<A,B>&& tupl_tie<ties>&& copy_assignable<E...>)
  { assign_from(r); }

  // assign ties = {} empty list assignment
  constexpr void operator=(std::true_type) const
    noexcept(types_all<is_nothrow_empty_list_assignable,ties>)
    requires (tupl_tie<ties> && types_all<is_empty_list_assignable,ties>)
  {
    map(as_tupl_t(*this),
        [](auto&...t)
          noexcept(types_all<is_nothrow_empty_list_assignable,ties>)
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
        [this](auto&...u)
          noexcept((is_nothrow_assignable_v<E&,U> && ...))
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
    { return {get<I>((decltype(t))t)...}; }

#include "namespace.hpp"

#endif
