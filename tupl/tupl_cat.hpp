/*
 SPDX-FileCopyrightText: 2024 The Lemuriad <wjwray@gmail.com>
 SPDX-License-Identifier: BSL-1.0
 Repo: https://github.com/Lemuriad/tupl
*/
#include "index_sequences.hpp"
#include "tupl.hpp"

#include "tupl_platform.hpp"

#include "namespace.hpp"

namespace impl {

// Helpers to compute concatenated tupl types

// cat_elem_t<ij, tuplish...>: the jth type of the ith tupl argument
//                             ij is a struct with {i,j} index members
//
template <auto ij, tuplish...TL>
using cat_elem_t = type_list_element_t<ij.j,
                   type_pack_element_t<ij.i, TL...>>;

// cat_t<tuplish..., ij...>: the type of the concatenated tupl of tuplish...
//                           given a list of ij indices of all elements
//
template <tuplish...TL, auto...ij>
auto cat_t(val_seq<ij_t, ij...>) -> tupl<cat_elem_t<ij,TL...>...>;

// ctad_t<X,V>: the template argument type V' deduced by X's CTAD for an
//              initializer of type V; X{declval<V>()} -> X<V'>
//              Assumes same_as< remove_cvref<V>, remove_cvref<V'> >
//
// => X is a tuplish template X<T...> with CTAD that conserves value types
// (unlike std::make_tuple which decays arguments and reference_wrappers).
//
template <template<typename...>class X, typename V>
using ctad_t = copy_cvref_t<decltype(
                 decltype(X{std::declval<all_extents_removed_t<V>>()})::x0
                                    ), V>;

// cat_ctad_t<X, tuplish..., ij...>: given a list of ij tupl element indices
//               return the type of the concatenated tupl using X's CTAD
//
template <template<typename...>class X, tuplish...TL, auto...ij>
auto cat_ctad_t(val_seq<ij_t,ij...>) -> X<ctad_t<X,cat_elem_t<ij,TL...>>...>;

} // impl

// cat_t<TL...> -> tupl<E...> concatenation type of tuplish element types
//
template <tuplish...TL>
using cat_t = decltype(impl::cat_t<std::remove_cvref_t<TL>...>
                                   (ij_seq<tupl_size_v<TL>...>{}));

// cat_ctad_t<X,TL...> -> decltype( X{fwd<E>()...} )
// concatenation type of forwarded element types as deduced by X's CTAD
//
template <template<typename...>class X, tuplish...TL>
using cat_ctad_t = decltype(impl::cat_ctad_t<X,tupl_fwd_t<TL>...>
                                      (ij_seq<tupl_size_v<TL>...>{}));

/*
  tupl_init; tupl aggregate initialization function overloads

  tupl_init<X,T...>(v...) -> X<T...>{v...};

  Workaround 'maker function' to deal with possible array initializers.
  The tupl elements are move-or-copy initialized from args v...
  exploding any array initialization into its array element initializers
  via integer_sequence expansion (so there's a size limit in practice)
  and brace elision (missing-braces warnings on Clang are suppressed).
*/
template <template<typename...>class X = tupl, typename...T>
constexpr auto tupl_init(auto&&...v)
            noexcept((is_nothrow_constructible_v<T,decltype(v)> && ...))
  -> X<T...>
            requires (sizeof...(T) == sizeof...(v)
                      && (is_constructible_v<T,decltype(v)> && ...))
{
  // if aggregate initialization works then use it
  if constexpr (requires {X<T...>{(decltype(v))v...};})
    return {(decltype(v))v...};

  // else there are array-valued arguments that fail to initialize
  else
    return [&]<auto...ij>(val_seq<ij_t, ij...>)
            noexcept((is_nothrow_constructible_v<T,decltype(v)> && ...))
          -> X<T...>
    {
      // Aggregate initialize with arrays expanded as flat lists of elements
      NO_WARN_MISSING_BRACES() // Clang warns of brace-elided initialization
      return {flat_index(get<ij.i>(fwds{(decltype(v))v...}),ij.j)...};
      END_NO_WARN_MISSING_BRACES()
    }(ij_seq<flat_size<std::remove_cvref_t<T>>...>{});
}
/*
  tupl_init(v...) -> tupl{v...};  with tupl CTAD
  tupl_init<X>(v...) -> X{v...};  with template id X CTAD
*/
template <template<typename...>class X = tupl, typename...T>
constexpr auto tupl_init(auto&&...v) noexcept(
     noexcept(tupl_init<X,impl::ctad_t<X,decltype(v)>...>((decltype(v))v...)))
  -> X<impl::ctad_t<X,decltype(v)>...>
    requires (sizeof...(T) == 0 && sizeof...(v) != 0 &&
    requires{tupl_init<X,impl::ctad_t<X,decltype(v)>...>((decltype(v))v...);})
{
  return tupl_init<X,impl::ctad_t<X,decltype(v)>...>((decltype(v))v...);
}

// cat(t...) concatenate tuplish t's preserving element types (no CTAD)
//
template <tuplish...TL>
constexpr auto cat(TL&&...tl)
 noexcept((std::is_nothrow_constructible_v<tupl_t<TL>,tupl_like_t<TL>> && ...))
 -> cat_t<tupl_t<TL>...>
 requires(std::is_constructible_v<tupl_t<TL>,tupl_like_t<TL>> && ...)
{
  return [&]<auto...ij>(val_seq<ij_t,ij...>)
   noexcept((std::is_nothrow_constructible_v<tupl_t<TL>,tupl_like_t<TL>>&&...))
 //-> cat_t<tupl_t<TL>...>
  {
    return tupl_init<tupl, impl::cat_elem_t<ij, tupl_t<TL>...>...>
                      (get<ij.j>(get<ij.i>(fwds{(TL&&)tl...}))...);
  }
  (ij_seq<tupl_size_v<TL>...>{});
}

// cat<X>(t...) concatenate tuplish t's to X{v...} using X's CTAD
//
template <template<typename...>class X, tuplish...TL>
constexpr auto cat(TL&&...tl) noexcept(
  (std::is_nothrow_constructible_v<cat_ctad_t<X,TL&&>,
                apply_cvref_t<TL&&,cat_ctad_t<X,TL&&>>> && ...))
 -> cat_ctad_t<X,TL&&...>
 requires (std::is_constructible_v<cat_ctad_t<X,TL&&>,
                apply_cvref_t<TL&&,cat_ctad_t<X,TL&&>>> && ...)
{
  return [&]<auto...ij>(val_seq<ij_t,ij...>) noexcept(
     (std::is_nothrow_constructible_v<cat_ctad_t<X,TL&&>,
                   apply_cvref_t<TL&&,cat_ctad_t<X,TL&&>>> && ...))
    //-> cat_ctad_t<X,TL&&...>
  {
    return tupl_init<X>(get<ij.j>(get<ij.i>(fwds{(TL&&)tl...}))...);
  }
  (ij_seq<tupl_size_v<TL>...>{});
}

#include "namespace.hpp"

#include "tupl_platform.hpp"
