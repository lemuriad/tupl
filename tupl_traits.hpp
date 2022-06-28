//  Copyright (c) 2022 Lemurian Labs https://lemurianlabs.com/
//
//  Distributed under the Boost Software License, Version 1.0.
//        http://www.boost.org/LICENSE_1_0.txt
//
//  Repo: https://github.com/Lemurian-Labs/tupl

#ifndef LML_TUPL_TRAITS_HPP
#define LML_TUPL_TRAITS_HPP

#include "array_compare.hpp"
#include "array_assign.hpp"

#include "namespace.hpp"

using std::size_t;

// same_ish<T,U> concept utility, removes cvref to match qualifed types
template <typename T, typename U>
concept same_ish = std::same_as<U, std::remove_cvref_t<T>>;

// type_list_size<T> the number of elements E in type list T = L<E...>
//
template <typename T> extern const size_t type_list_size;
//
template <template <typename...> class Tupl, typename...E>
inline constexpr auto type_list_size<Tupl<E...>> = sizeof...(E);

//
// types_all<LT,P> meta function -> (P<T>() && ...) for LT = L<T...>
//
template <typename LT, template <typename> class P>
extern const bool types_all;
//
template <template <typename...> class L, typename...T,
          template <typename> class P>
inline constexpr bool types_all<L<T...>,P> = (P<T>() && ...);

//
// type_map<LT,M> -> L<M<T>...> for type list LT = L<T...>
//
template <template <typename...> class M, typename LT>
struct type_map;
//
template <template <typename...> class M,
          template <typename...> class L, typename...T>
struct type_map<M, L<T...>> {
  using type = L< M<T>... >;
};

// type_pack_indexof<X,E...> Index of element of type X in pack E...
// Fails with a static_assert if there's no X or multiple Xs in E...
//
template <typename X, typename...E>
inline constexpr int type_pack_indexof = []
{
  static_assert((std::same_as<X,E> + ...) == 1,
    "type_pack_indexof error: pack must have a single matching type");
  int index = 0;
  int count = 0;
  ((std::same_as<X,E> ? index = count : ++count), ...);
  return index;
}();

#ifdef __has_builtin
#if __has_builtin(__type_pack_element)
#define TYPEPACKEL(I,...) __type_pack_element<I,__VA_ARGS__>
#endif
#endif

#ifndef TYPEPACKEL

namespace impl {

template <size_t I, template<typename> typename W,
  typename X0=void,typename X1=void,typename X2=void,typename X3=void,
  typename X4=void,typename X5=void,typename X6=void,typename X7=void,
  typename X8=void,typename X9=void,typename Xa=void,typename Xb=void,
  typename Xc=void,typename Xd=void,typename Xe=void,typename Xf=void,
  typename...T>
consteval auto type_pack_element()
{
  switch (I)
  {
  case 0x0: if constexpr (I==0x0) return W<X0>{}; UNREACHABLE();
  case 0x1: if constexpr (I==0x1) return W<X1>{}; UNREACHABLE();
  case 0x2: if constexpr (I==0x2) return W<X2>{}; UNREACHABLE();
  case 0x3: if constexpr (I==0x3) return W<X3>{}; UNREACHABLE();
  case 0x4: if constexpr (I==0x4) return W<X4>{}; UNREACHABLE();
  case 0x5: if constexpr (I==0x5) return W<X5>{}; UNREACHABLE();
  case 0x6: if constexpr (I==0x6) return W<X6>{}; UNREACHABLE();
  case 0x7: if constexpr (I==0x7) return W<X7>{}; UNREACHABLE();
  case 0x8: if constexpr (I==0x8) return W<X8>{}; UNREACHABLE();
  case 0x9: if constexpr (I==0x9) return W<X9>{}; UNREACHABLE();
  case 0xa: if constexpr (I==0xa) return W<Xa>{}; UNREACHABLE();
  case 0xb: if constexpr (I==0xb) return W<Xb>{}; UNREACHABLE();
  case 0xc: if constexpr (I==0xc) return W<Xc>{}; UNREACHABLE();
  case 0xd: if constexpr (I==0xd) return W<Xd>{}; UNREACHABLE();
  case 0xe: if constexpr (I==0xe) return W<Xe>{}; UNREACHABLE();
  case 0xf: if constexpr (I==0xf) return W<Xf>{}; UNREACHABLE();
  default: if constexpr (I>=0x10)
             return type_pack_element<I - 0x10, W, T...>();
  }
}
} // impl
#define TYPEPACKEL(I,...) typename decltype(impl::type_pack_element<I, \
std::type_identity,E...>())::type

#endif

// type_pack_element<I,T...> alias of the Ith element in type pack T...
//
template <size_t I, typename...E> requires (I < sizeof...(E))
using type_pack_element = TYPEPACKEL(I,E...);

#undef TYPEPACKEL

// comparable trait wrappers for array_compare concepts
//
template <typename T> using is_three_way_comparable
         = std::bool_constant< three_way_comparable<T>>;

template <typename T> using is_equality_comparable
         = std::bool_constant< equality_comparable<T>>;

template <typename T> using is_member_default_3way
         = std::bool_constant< member_default_3way<T>>;

// miscellaneous helper traits, dumped in impl namespace for now
//
namespace impl {

template <typename T> concept move_assignable_object
                    = std::is_move_assignable_v<T>
                   && std::is_object_v<T>
                   && ! const_assignable<T>;

template <typename T>
using is_value = std::bool_constant<std::is_object_v<T>
                                 && ! const_assignable<T>>;

template <typename U, typename T>
concept constructs = std::is_constructible_v<
           remove_all_extents<T>,
           remove_all_extents<std::remove_cvref_t<U>>>
         && same_extents<std::remove_cvref_t<U>,T>;

template <typename T, typename U>
using is_convertible
 = std::is_convertible<remove_all_extents<std::remove_cvref_t<T>>,
                       remove_all_extents<U>>;

template <typename U, typename T = U>
concept direct_initializes = requires (U u) {T{static_cast<U&&>(u)};};

template <typename U>
using is_direct_initialized = std::bool_constant<direct_initializes<U>>;

template <typename T> void test_default_list_initializable(T);

template <typename T> concept default_list_initializable
            = requires { test_default_list_initializable<T>({}); };

//template <typename T> using is_move_assignable_object
//          = std::bool_constant<move_assignable_object<T>>;
//
} // impl

#include "namespace.hpp"

#endif
