//  Copyright (c) 2023 Lemurian Labs https://lemurianlabs.com/
//
//  Distributed under the Boost Software License, Version 1.0.
//        http://www.boost.org/LICENSE_1_0.txt
//
//  Repo: https://github.com/Lemurian-Labs/tupl

#ifndef LML_TUPL_TIE_HPP
#define LML_TUPL_TIE_HPP

#include "tupl.hpp"

#include "namespace.hpp"

/*
  ties<E...> derives from tupl<E...> and adds operator=(rhs) overloads

  1  operator=(same_ish<ties>) Replaces deleted default assignments
                               (the only non-const qualified operator=)
  2  operator=(tuplish) const  move or copy assigns from other tuplish
  3a operator=({rvals}) const  move-assigns from init-list of rvalues
  3b operator=({lvals}) const  copy-assigns from init-list of lvalues
  4  operator=({}) const       assign from empty init list, i.e. 'clear'
*/
template <typename...E> struct ties : tupl<E...>
{
  // assign ties = ties replaces deleted default assign, non-const
  template <same_ish<ties> Ties>
  constexpr auto& operator=(Ties&& r)
    noexcept(noexcept(assign_to{*this} = r))
    requires tupl_tie<ties>
  { return assign_to{*this} = r; }

  // assign ties = tuplish, allowing only non-narrowing conversions
  template <tuplish T>
  constexpr auto& operator=(T&& r) const
    noexcept(noexcept(assign_to{*this} = r))
    requires types_all<is_non_narrow_const_assignable,ties,tupl_t<T>>
  { return assign_to{*this} = r; }

  // assign ties = tupl_move rvalue init list; move is better match
  template <typename...>
  constexpr auto& operator=(tupl_move_t<ties> r) const
    noexcept(noexcept(assign_to{*this} = r))
    requires (tupl_tie<ties> && move_assignable<E...>)
  { return assign_to{*this} = r; }

  // assign ties = tupl_view lvalue init list; copy is worse match
  template <typename A = tupl_view_t<ties>, typename B = A>
  constexpr auto& operator=(A const& r) const
    noexcept(noexcept(assign_to{*this} = r))
    requires(std::same_as<A,B>&& tupl_tie<ties>&& copy_assignable<E...>)
  { return assign_to{*this} = r; }

  // assign ties = {} empty list assignment
  template<typename...>
  constexpr auto& operator=(std::true_type) const
    noexcept(noexcept(assign_to{*this} = std::true_type{}))
    requires tupl_tie<ties>
  { return assign_to{*this} = std::true_type{}; }
};

// ties CTAD, deduces forwarding references
//
template <typename...E> ties(E&&...) ->  ties<E&&...>;

//
// tie(a...) -> ties<decltype(a)&...>{a...}; rejects rvalue arguments
//
template <typename...T>
constexpr auto tie(T&...t) noexcept -> ties<T&...> const
{
    return { t... };
}
//
// tie_fwd(a...) -> ties<decltype(a)...>{a...} const
//                     same as CTAD ties{a...} but const qualified
template <typename...T>
constexpr auto tie_fwd(T&&...t) noexcept -> ties<T&&...> const
{
    return { (T&&)t... };
}
//
// getie<I...>(tupl) -> tie(get<I>(tupl)...)
//
template <unsigned...I>
constexpr auto getie(tuplish auto&& t) noexcept
  -> ties<decltype(get<I>((decltype(t))t))...> const
     requires ((I < tupl_size<decltype(t)>) && ...)
    { return {get<I>((decltype(t))t)...}; }

#include "namespace.hpp"

#endif
