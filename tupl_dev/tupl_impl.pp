#ifndef TUPL_PASS
/*
     Copyright (c) 2022 Lemurian Labs https://lemurianlabs.com/

     Distributed under the Boost Software License, Version 1.0.
           http://www.boost.org/LICENSE_1_0.txt

     Repo: https://github.com/Lemurian-Labs/tupl
*/
#if 0 /* Comments after # directives are removed in preprocessed output

  tupl_impl.pp -> tupl_impl.hpp preprocessing stage

  A tupl implementation tupl_impl.hpp is generated by preprocessing
  this file using the IREPEAT library to expand tuple-arity repetitions
  [0 .. tupl_max_arity) and each tupl member element within that arity.
  Preprocessing proceeds in three passes, TUPL_PASS symbol = empty,1,2.

  The resulting output tupl_impl.hpp implementation header depends on a
  prior #include "array_compare.hpp" - see tupl.hpp for setup required;
  symbols UNREACHABLE and NUA remain in the preprocessed output, for
  platform-specific handling of unreachable(), no_unique_address, etc.
*/
#endif

#include "IREPEAT.hpp"

#include "tupl_impl_noedit_warn.hpp" // Warning banner "generated file!"
//
#ifndef TUPL_MAX_ARITY /* the maximum number of elements specified as
                          parenthesized hex digits so (1)(0) == 16 */
#define TUPL_MAX_ARITY (1)(0)
#endif

//
inline constexpr size_t tupl_max_arity = HEXLIT(TUPL_MAX_ARITY);
/*
   tupl<E...> primary template declaration; an aggregate struct
   with [[no_unique_address]] attribute on all element member decls E...
*/
template <typename...E> struct tupl;  // aggregate [[no_unique_address]]
//
// tupl CTAD deduces elements by-value, including arrays (and functions)
//
template <typename...E> tupl(E const&...) -> tupl<E...>;
//
template<typename T>  inline constexpr bool is_tupl_v = false;
template<typename...E>inline constexpr bool is_tupl_v<tupl<E...>>{true};
//
template<typename T>  inline constexpr bool is_lupl_v = false;
#if !defined(NO_LUPL) // lupl defined unless NO_LUPL symbol is #defined
/*
   lupl<E...> 'layout compatible' tupl without [[no_unique_address]]
*/
template <typename...E> struct lupl;  // aggregate, layout-compatibility
template <typename...E> lupl(E const&...) -> lupl<E...>;
template<typename...E>inline constexpr bool is_lupl_v<lupl<E...>>{true};
#endif
//
template<typename T>
concept tupl_or_lupl = is_tupl_v<tupl_t<T>> || is_lupl_v<tupl_t<T>>;
/*
   Derived tuplish types with custom CTAD only, nothing else added
*/
template <typename...E> struct fwds  : tupl<E...> {}; // forwarding refs
template <typename...E> struct lvals : tupl<E...> {}; // lvalue refs
template <typename...E> struct rvals : tupl<E...> {}; // rvalue refs
template <typename...E> struct cvals : tupl<E...> {}; // const view vals
//
// CTAD guides
//
template <typename...E> fwds(E&&...) -> fwds<E&&...>;
template <typename...E> lvals(E&...) -> lvals<E&...>;
template <rvalue... RV> rvals(RV&&...) -> rvals<RV&&...>;
template <typename...E> cvals(E const&...) -> cvals<tupl_view_t<E>...>;
//

#define TUPL_TYPE_ID XD
#define TUPL_DATA_ID xD
#define MEMBER_DECL(D) TUPL_TYPE_ID(D) TUPL_DATA_ID(D);

#define TUPL_PASS 1
//
// tupl<T...> specializations VREPEATed for each arity
#define TUPL_ID tupl
#define VREPEAT_COUNT TUPL_MAX_ARITY
#define VREPEAT_MACRO tupl_impl.pp
#include "VREPEAT.hpp"
#undef TUPL_ID
#undef NUA
//
// lupl<T...> specializations VREPEATed for each arity
#if !defined(NO_LUPL) // conditionally don't generate lupl
#define NUA
#define TUPL_ID lupl
#define VREPEAT_COUNT TUPL_MAX_ARITY
#define VREPEAT_MACRO tupl_impl.pp
#include "VREPEAT.hpp"
#undef TUPL_ID
#undef NUA
#endif

#undef TUPL_PASS
#define TUPL_PASS 2
#include "tupl_impl.pp"

#elif (TUPL_PASS == 1) // Repeat this section for each arity

#if NREPEAT == 0
#define NZREPEAT(...)
#define MAYBE_UNUSED0()[[maybe_unused]]
#else
#define NZREPEAT(...)__VA_ARGS__
#define MAYBE_UNUSED0()
#endif

#define MEMBER_FWDS(t) XREPEAT(VREPEAT_INDEX,t.TUPL_DATA_ID,COMMA)

//
template <XREPEAT(VREPEAT_INDEX,class TUPL_TYPE_ID,COMMA)>
struct TUPL_ID<XREPEAT(VREPEAT_INDEX,TUPL_TYPE_ID,COMMA)>
{
 using tupl_t = TUPL_ID;
 //
 XREPEAT(VREPEAT_INDEX,NUA MEMBER_DECL,NOSEP)
 //
 friend auto operator<=>(TUPL_ID const&,TUPL_ID const&)
  NZREPEAT(requires types_all<is_member_default_3way,TUPL_ID>)= default;
 //
 template <same_ish<TUPL_ID> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(MAYBE_UNUSED0()T&& t, F f)noexcept(
  noexcept(f(MEMBER_FWDS(U(t)))))
  { return f(MEMBER_FWDS(U(t))); }
};

#undef MEMBER_FWDS
#undef MAYBE_UNUSED0
#undef NZREPEAT

#else // TUPL_PASS == 2

/*
  get<I>(t)
*/
template <size_t I, tuplish T>
constexpr auto get(T&& t) noexcept
-> apply_cvref_t<T&&,type_list_element_t<I,tupl_t<T>>>
  requires (I < tupl_size<T>)
{
  using tuplt = tupl_t<T>;
  using ret_t = apply_cvref_t<T&&,type_list_element_t<I,tuplt>>;
  using ptr_t = std::remove_reference_t<ret_t>*;

  struct E{constexpr E(...){}};
  struct At{ E a[I+1]; ptr_t e; E b[tupl_size<tuplt>-I]; };

  return map(static_cast<tupl_like_t<T>&>(t), [](auto&...a) noexcept
    -> ret_t
  {
    NO_WARN_MISSING_BRACES(
      return static_cast<ret_t>(*At{0,&a...,0}.e);
    )
  });
}

/*
   get<T>(t)
*/
template <typename X>
constexpr auto&& get(tuplish auto&& t) noexcept
{
  return get<type_list_indexof<X, tupl_t<decltype(t)>>>((decltype(t))t);
}

/*
  tupl_mptr<I,T> member pointer getter
*/
template <size_t I, typename T> // defined for tupl or lupl derived
auto tupl_mptr = NOT_DEFINED(tupl_mptr<I,T>);

template <size_t I, typename T>
  requires tupl_or_lupl<tupl_t<T>>
constexpr type_list_element_t<I,tupl_t<T>> T::* tupl_mptr<I,T>
 = [] {
  static_assert(I < tupl_size<T>, "tupl_mptr index out of bounds");
# define ELSE() else
# define MACRO(N) if constexpr(I==HEXLIT(N))return&T::TUPL_DATA_ID(N);
  XREPEAT(TUPL_MAX_ARITY,MACRO,ELSE)
# undef MACRO
# undef ELSE
  UNREACHABLE()
}();

/*
  tupl_mptrs<T> return a tupl of member pointers
*/
template <typename T, size_t...I>
  requires tupl_or_lupl<tupl_t<T>>
constexpr auto tupl_mptrs = tupl_mptrs<T,I...,sizeof...(I)>;
//
template <typename T, size_t...I>
  requires (tupl_or_lupl<tupl_t<T>> && tupl_size<T> == sizeof...(I))
constexpr auto tupl_mptrs<T,I...> = tupl{tupl_mptr<I,T>...};

#include "tupl_impl_assign.hpp"
#include "tupl_impl_compare.hpp"
#include "tupl_impl_cat.hpp"

#include "IREPEAT_UNDEF.hpp"

#undef MAP_IMPL

#undef MEMBER_DECL
#undef TUPL_DATA_ID
#undef TUPL_TYPE_ID

#undef TUPL_MAX_ARITY

#undef TUPL_PASS

#endif
