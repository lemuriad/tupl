//  Copyright (c) 2022 Lemurian Labs https://lemurianlabs.com/
//
//  Distributed under the Boost Software License, Version 1.0.
//        http://www.boost.org/LICENSE_1_0.txt
//
//  Repo: https://github.com/Lemurian-Labs/tupl

#ifndef LML_TUPL_TRAITS_HPP
#define LML_TUPL_TRAITS_HPP

#include "array_assign.hpp"  // is_assignable traits

#include "namespace.hpp"

// tupl_t<Tupl> alias of the member typedef tupl_t
//
template <typename Tupl>
using tupl_t = typename std::remove_cvref_t<Tupl>::tupl_t;

// tupl_like_t<Tupl> tupl_t with same cvref qualifiers as Tupl
//
template <typename Tupl>
using tupl_like_t = copy_cvref_t<Tupl,tupl_t<Tupl>>;

// tuplish concept, requires complete type T, possibly cvref qualified,
//  with a member alias 'tupl_t' to which T is implicitly convertible
// (e.g. by deriving from tupl) and a map function such that
//  map(tupl,functor) returns functor(x...) called on tupl members x...
//
// Here, implicit conversion T -> tupl_t is tested by reference binding
// and a void functor is used to test for presence of a map function
//
template <typename T>
concept tuplish = requires (T& v) {
  {map(tupl_like_t<T&>(v), [](auto&&...){})} -> std::same_as<void>;
};

// as_tupl_t(tup) cast to tupl_like_t (fwd the arg for value category)
//
template <tuplish T>
constexpr auto as_tupl_t(T&& t) noexcept -> tupl_like_t<T&&>
{ return static_cast<tupl_like_t<T&&>>(t); }

// type_list
//
template <typename...> struct type_list {};

// type_list_size<T> the number of elements E in type list T = L<E...>
//
template <typename T> extern const size_t type_list_size;
//
template <template <typename...> class L, typename...E>
inline constexpr auto type_list_size<L<E...>> = sizeof...(E);

// tupl_size<Tupl> number of tupl elements (requires complete type)
//
template <tuplish Tupl> inline constexpr auto tupl_size
                                       = type_list_size<tupl_t<Tupl>>;

// types_all<LT,P> meta function -> (P<T>() && ...) for LT = L<T...>
//
template <template <typename...> class P, typename...T>
inline constexpr auto types_all
         = requires {{types_all<P,T...>}->std::same_as<bool>;};
//
template <template <typename...> class L, typename...T,
          template <typename...> class P>
inline constexpr bool types_all<P,L<T...>> = (P<T>() && ...);
//
template <template <typename...> class L, typename...T,
          template <typename...> class R, typename...U,
          template <typename...> class P>
  requires (sizeof...(T) == sizeof...(U))
inline constexpr bool types_all<P,L<T...>,R<U...>> = (P<T,U>() && ...);

template <typename...E>
concept move_assignable = (is_move_assignable_v<E> && ...);

template <typename...E>
concept copy_assignable = (is_copy_assignable_v<E> && ...);

template <typename L, typename R>
using is_lval_assignable = is_assignable<L&,R>;

template <typename L, typename R>
using is_lval_nothrow_assignable = is_nothrow_assignable<L&,R>;

// non_narrow_assignable<T> 'uniform' aggregate assignment property
//
template <typename L, typename R = L>
concept non_narrow_assignable
  = requires (all_extents_removed_t<L> l
             ,all_extents_removed_t<R> r) { l={r}; };
//
template <typename L, typename R>
using is_non_narrow_assignable = std::bool_constant<
         non_narrow_assignable<L,R> >;

// const_assignable<T> captures the property of references and proxies
// that they can be assigned to even if const qualified
//
template <typename L, typename R = L>
concept const_assignable
  = requires (all_extents_removed_t<L> const lc
             ,all_extents_removed_t<R> r) { lc=r; };
//
template <typename L, typename R = L>
using is_const_assignable = std::bool_constant<
         const_assignable<L,R> >;

// non_narrow_const_assignable<T> references and proxies can be assigned
// to even if const qualified - this also enforces non-narrowing
//
template <typename L, typename R>
concept non_narrow_const_assignable
  = requires (all_extents_removed_t<L> const lc
             ,all_extents_removed_t<R> r) { lc={r}; };
//
template <typename L, typename R = L>
using is_non_narrow_const_assignable = std::bool_constant<
         non_narrow_const_assignable<L,R> >;

// is_value<T>: T is an object type excluding const_assignable types
//
template <typename T>
using is_value = std::bool_constant<std::is_object_v<T>
                                 && ! const_assignable<T>>;

// tupl_val<T> concept: tuplish type with all elements object-type
// (note: is_object matches unbounded array T[], allows for FAM)
//
template <typename Tupl>
concept tupl_val
      = tuplish<Tupl>
     && types_all<is_value, tupl_t<Tupl>>;

// tupl_tie<T> concept: matches tupl of reference-like elements detected
// as const-assignable; of assignable reference or proxy-reference type.
// (note: const_assignable matches unbounded array reference T(&)[])
//
template <typename Tupl>
concept tupl_tie
      = tuplish<Tupl>
     && tupl_size<Tupl> != 0
     && types_all<is_const_assignable, tupl_t<Tupl>>;

// type_map<M,LT> -> L<M<T>...> for type list LT = L<T...>
//
template <template <typename...> class M, typename LT>
struct type_map;
//
template <template <typename...> class M,
          template <typename...> class L, typename...T>
struct type_map<M, L<T...>> {
  using type = L< M<T>... >;
};
//
template <template <typename...> class M, typename LT>
using type_map_t = typename type_map<M,LT>::type;

// tupl_fwd_t<t> apply the top-level cvref of tuplish t to its elements
//   -> tupl<apply_cvref_t<t,E>...> where E... are element types of t
//
template <tuplish t>
struct tupl_fwd {
  template <typename T> using apply_cvref = apply_cvref_t<t,T>;
  using type = type_map_t<apply_cvref, tupl_t<t>>;
};
//
template <tuplish t>
using tupl_fwd_t = typename tupl_fwd<t>::type;

// type_pack_indexof<X,E...> Index of element of type X in pack E...
// Fails with a static_assert if there's no X or multiple Xs in E...
//
template <typename X, typename...E>
inline constexpr int type_pack_indexof = [] {
  static_assert(sizeof...(E) > 0, "type_pack_indexof zero-size pack");
  static_assert((std::same_as<X,E> + ...) == 1,
    "type_pack_indexof error: pack must have a single matching type");
  int k=0, r=-1;
  return ((std::same_as<X,E> && r<0 ? r=k : ++k),...), r;
}();

// type_list_indexof<X,TL> Index of element of type X in type list TL
//
template <typename X, typename TL> extern const int type_list_indexof;
//
template <typename X, template <typename...> class L, typename...E>
inline constexpr int type_list_indexof<X,L<E...>>
                   = type_pack_indexof<X,E...>;

// type_pack_element_t; Clang has a builtin, use it if available
//
#ifdef __has_builtin
#if __has_builtin(__type_pack_element)
#define TYPEPACKEL __type_pack_element<I,E...> // I,E... are 'baked in'
#endif
#endif
//
// else implement type_pack_element_t
//
#ifndef TYPEPACKEL
//
namespace impl {
//
template <
  typename X0=void,typename X1=void,typename X2=void,typename X3=void,
  typename X4=void,typename X5=void,typename X6=void,typename X7=void,
  typename X8=void,typename X9=void,typename Xa=void,typename Xb=void,
  typename Xc=void,typename Xd=void,typename Xe=void,typename Xf=void,
  typename...T>
constexpr decltype(auto) type_pack_element_t = []<size_t I>()
{
  switch (I)
  {
  case 0x0: if constexpr (I==0x0) return std::type_identity<X0>{};
  case 0x1: if constexpr (I==0x1) return std::type_identity<X1>{};
  case 0x2: if constexpr (I==0x2) return std::type_identity<X2>{};
  case 0x3: if constexpr (I==0x3) return std::type_identity<X3>{};
  case 0x4: if constexpr (I==0x4) return std::type_identity<X4>{};
  case 0x5: if constexpr (I==0x5) return std::type_identity<X5>{};
  case 0x6: if constexpr (I==0x6) return std::type_identity<X6>{};
  case 0x7: if constexpr (I==0x7) return std::type_identity<X7>{};
  case 0x8: if constexpr (I==0x8) return std::type_identity<X8>{};
  case 0x9: if constexpr (I==0x9) return std::type_identity<X9>{};
  case 0xa: if constexpr (I==0xa) return std::type_identity<Xa>{};
  case 0xb: if constexpr (I==0xb) return std::type_identity<Xb>{};
  case 0xc: if constexpr (I==0xc) return std::type_identity<Xc>{};
  case 0xd: if constexpr (I==0xd) return std::type_identity<Xd>{};
  case 0xe: if constexpr (I==0xe) return std::type_identity<Xe>{};
  case 0xf: if constexpr (I==0xf) return std::type_identity<Xf>{};
  default: if constexpr (I>=0x10) return type_pack_element_t<T...>.
                                         template operator()<I-0x10>();
  }
};
#define TYPEPACKEL typename decltype(impl::type_pack_element_t<E...>\
.template operator()<I>())::type; // I,E... are 'baked in'
} // impl

#endif

// type_pack_element_t<I,T...> alias of element I in type pack T...
//
template <size_t I, typename...E> requires (I < sizeof...(E))
using type_pack_element_t = TYPEPACKEL; // TYPEPACKEL 'bakes in' I,E...

#undef TYPEPACKEL

// type_list_element<I,TL> type_identity of element I in type list TL
//
template <size_t I, typename TL>
extern std::type_identity<void> type_list_element;
//
template <size_t I, template <typename...> class L, typename...E>
std::type_identity<type_pack_element_t<I,E...>>
                   type_list_element<I,L<E...>>{};

// type_list_element_t<I,TL> type of element I in type list TL = L<E...>
//
template <size_t I, typename TL>
using type_list_element_t = typename decltype(
                            type_list_element<I,TL>)::type;

namespace impl {

// Helpers to compute tupl_move_t, recursively
//
template <typename U> auto tmove(...) -> std::remove_cvref_t<U>&&;
template <typename U> auto tmove(...) -> std::remove_cvref_t<U>
  requires (sizeof(U)<=16 && !c_array<U> && std::is_trivially_copyable_v
                                             <std::remove_cvref_t<U>>);
template<typename U> struct tmovet;
template<typename U> using tmove_t = typename tmovet<U>::type;
template<tuplish U>  auto tmove(int) -> type_map_t<tmove_t,tupl_t<U>>;
template<typename U> void tmove(int) requires(!requires{tmove<U&>(0);});
template<typename U> struct tmovet {using type=decltype(tmove<U>(0));};

// Helpers to compute tupl_view_t, recursively
//
template <typename U> auto tview(...) -> std::remove_cvref_t<U> const&;
template <typename U> auto tview(...) -> std::remove_cvref_t<U>
  requires (sizeof(U)<=16 && !c_array<U> && std::is_trivially_copyable_v
                                             <std::remove_cvref_t<U>>);
template<typename U> struct tviewt;
template<typename U> using tview_t = typename tviewt<U>::type;
template<tuplish U>  auto tview(int) -> type_map_t<tview_t,tupl_t<U>>;
template<typename U> void tview(int) requires(!requires{tview<U&>(0);});
template<typename U> struct tviewt {using type=decltype(tview<U>(0));};

} // impl

/*
 tupl_move_t<U> -> U&&
 tupl_view_t<U> -> U const&
             or -> U       for small non-array trivially-copyable types
             or -> void    for non-referencable types
             or if U is a tupl, recursively transform its element types
*/
template <typename U> using tupl_move_t = impl::tmove_t<U>;
template <typename U> using tupl_view_t = impl::tview_t<U>;

#include "namespace.hpp"

#endif
