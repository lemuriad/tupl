#ifndef TUPL_PASS
/*
     Copyright (c) 2022 Lemurian Labs https://lemurianlabs.com/

     Distributed under the Boost Software License, Version 1.0.
           http://www.boost.org/LICENSE_1_0.txt

     Repo: https://github.com/Lemurian-Labs/tupl
*/
#if 0 /*
   tupl_impl_pre.hpp -> tupl_impl.hpp

   A tupl implementation is generated by preprocessing this file,
   using the IREPEAT library to expand repetitions for each tuple arity
   [0 .. tupl_max_arity) and for each tupl element within that arity.
   Preprocessing proceeds in three passes, marked by TUPL_PASS symbol,
   starting empty
*/
#endif
#if __has_include("tupl_impl_noedit_warn.hpp") // Warning banner for
#include "tupl_impl_noedit_warn.hpp"           // the generated file
#endif                                         // "Don't edit this!"

#ifndef TUPL_ID      // TUPL_ID = your name for the tuple template id
#define TUPL_ID tupl // E.g. configure with -DTUPL_ID=tuple
#endif

#ifndef TUPL_MAX_ARITY        // TUPL_MAX_ARITY = max number of elements
#define TUPL_MAX_ARITY (1)(0) // specified as parenthesized hex digits
#endif

#ifndef TUPL_NUA // TUPL_NUA : define empty to disable no_unique_address
#define TUPL_NUA [[no_unique_address]]
#endif

#include "IREPEAT.hpp"

#define TUPL_MAX_ARITY_ID CAT(TUPL_ID,_max_arity)

#include "namespace.hpp" // Optional namespace, defaults to lml

inline constexpr std::size_t TUPL_MAX_ARITY_ID = HEXLIT(TUPL_MAX_ARITY);
//
// tupl primary template declaration
template <typename...T> struct TUPL_ID;
//
// CTAD deduce values including arrays (i.e. no decay, unlike std tuple)
template <typename...E> TUPL_ID(E const&...) -> TUPL_ID<E...>;
//
// tuplish concept, requires T is a complete type with a member alias
//  'tupl_t' to which T is implicitly convertible (e.g. by inheritance).
//  Note that tuplish matches tupl with cvref qualifiers
//
template <typename T> concept tuplish = requires (T v,
  typename std::remove_cvref_t<T>::tupl_t const& u) { decltype(u)(v); };
//
// tupl_size variable template (unrelated to std::tuple_size)
template <class T> extern const std::size_t tupl_size;
//
template <class T> requires (std::is_reference_v<T>||std::is_const_v<T>)
inline constexpr auto tupl_size<T> = tupl_size<std::remove_cvref_t<T>>;
//
template <template <typename...> class Tupl, typename...U>
inline constexpr auto tupl_size<Tupl<U...>> = sizeof...(U);
//
// size(tupl): the number of tupl elements as an integral constant
template <tuplish T> constexpr auto size(T const&)
   -> std::integral_constant<std::size_t,tupl_size<T>>{ return {}; }
//
// types_all meta function
template <typename, template <typename> class P>
extern const bool types_all;
//
template <template <typename...> class Tup, typename...T,
          template <typename> class P>
inline constexpr bool types_all<Tup<T...>,P> = (P<T>() && ...);
//
// tupl_val concept: tupl type with all object-type elements
// (note: is_object matches unbounded array T[], allows for FAM)
template <typename T>
concept tupl_val = tuplish<T>
     && types_all<std::remove_cvref_t<T>, std::is_object>;
//
// tupl_tie concept: matches tupls of reference-like elements detected
// as const-assignable; either reference-to-object assignable elements
// or assignable proxy-references.
// (note: const_assignable matches unbounded array reference T(&)[])
//
template <typename T>
concept tupl_tie =
        tuplish<T>
     && ! tupl_val<T> // eliminate empty tupl
     && types_all<std::remove_cvref_t<T>, is_const_assignable>;
//
// type_map<tupl<T...>, map> -> tupl<map<T>...>
template <template <typename...> class, typename>
struct type_map;
//
template <template <typename...> class M,
          template <typename...> class Tup, typename...T>
struct type_map<M, Tup<T...>> {
  using type = Tup< M<T>... >;
};
/*
  assign_fwd<U> 'forwarding' carrier type for assignment operator=
   -> void       for a non-assignable type (renders it incomplete)
           else V = remove_cvref_t<U> and:
   -> V          for trivially_copyable non-array V of 16 bytes or less
   -> V const&   for lvalue_ref U or array V (no move)
   -> V&&        otherwise (assumes rvalue move candidate)
     (V&& won't accept V&, so either move or const_cast to V const&)
 or
   -> tupl_assign_fwd_t<V> for a tuplish<U>, recursive

  tupl_assign_fwd<Tupl> maps each tupl element type to its assign_fwd<T>
*/
template <typename U> struct assign_fwd;
//
template <typename U> using assign_fwd_t = typename assign_fwd<U>::type;
//
template <typename Tupl>
using tupl_assign_fwd = type_map<assign_fwd_t,Tupl>;
//
template <typename Tupl>
using tupl_assign_fwd_t = typename tupl_assign_fwd<Tupl>::type;
//
namespace impl {
//
template <typename U>
auto assign_fwd_f()
{
  using V = std::remove_cvref_t<U>;
  if constexpr (tuplish<U>)
    return tupl_assign_fwd<V>{};
  else
  if constexpr (is_move_assignable_v<V>)
  {
    if constexpr (std::is_trivially_copyable_v<V> && !is_array_v<V>
                  && sizeof(V)<=16)
        return std::type_identity<V>{};
    else
    if constexpr (std::is_lvalue_reference_v<U> || is_array_v<V>)
        return std::type_identity<V const&>{};
    else
        return std::type_identity<V&&>{};
  }
  else
    return std::type_identity<void>{};
}
}
template <typename U>
struct assign_fwd : decltype(impl::assign_fwd_f<U>()) {};
//
template <typename Tupl>
using tupl_tie_t =
         typename type_map<std::add_lvalue_reference_t,Tupl>::type;
//
template <typename L, typename R, typename...>
inline constexpr bool tupls_assignable
                    = tupls_assignable<L,R,
                      std::remove_cvref_t<L>, std::remove_cvref_t<R>>;

template <typename L, typename R, typename...l, typename...r>
inline constexpr bool tupls_assignable<L,R,TUPL_ID<l...>,TUPL_ID<r...>>
                         = (assignable_from<l&, copy_ref<R,r>> && ...);
//
template <typename l, typename r, typename R>
inline constexpr bool noexcept_assignable = requires (l& h, r& s)
   {requires noexcept(assign(h,(copy_ref<R,r>)s));};
//
template <typename L, typename R, typename...>
inline constexpr bool tupls_noexcept_assignable
                    = tupls_noexcept_assignable<L,R,
                      std::remove_cvref_t<L>, std::remove_cvref_t<R>>;

template <typename L, typename R, typename...l, typename...r>
inline constexpr bool tupls_noexcept_assignable<L,R,
                                     TUPL_ID<l...>,TUPL_ID<r...>>
                         = (noexcept_assignable<l,r,R> && ...);
//
// assign_to<tupL> customization for tuple-tuple assignment
//
template <tuplish Lr>
struct assign_to<Lr>
{
  using L = std::remove_cvref_t<Lr>;

  using value_type = std::conditional_t<tupl_val<L>, L, const L>;

  value_type& l;

  template <tuplish R>
    requires (tupls_assignable<L, R&&>)
  constexpr value_type& operator=(R&& r) const noexcept(
    noexcept(tupls_noexcept_assignable<L,R&&>))
  {
    map(l, [&r](auto&...t) {
      map(r, [&t...](auto&...u) {
        if constexpr (std::is_lvalue_reference_v<R>)
          (assign(t,u), ...);
        else
          (assign(t,(std::remove_reference_t<decltype(u)>&&)(u)), ...);
      });
    });
    return l;
  }
};

template <typename... T>
constexpr void swap(TUPL_ID<T...>& l, decltype(l) r)
  noexcept((std::is_nothrow_swappable_v<T> && ...))
    requires (std::swappable<T> && ...)
{
  map(l, [&r](T&...t) {
    map(r, [&t...](T&...u)
    {
      (std::ranges::swap(t, u), ...);
    });
  });
}

namespace impl {
//
template <typename T> using assign_overload = std::bool_constant<
                            assignable_from<T&,T&>
                  && ! std::assignable_from<T&,T&>>;
//
template <typename L, typename R>
inline constexpr bool tupls_assignable {};
//
template <typename...U, typename...V>
inline constexpr bool tupls_assignable<TUPL_ID<U...>,TUPL_ID<V...>> =
                           (assignable_from<U&,V> && ...);

template <typename T> using is_three_way_comparable = std::bool_constant
                             < three_way_comparable<T> >;
template <typename T> using is_equality_comparable  = std::bool_constant
                             < equality_comparable<T> >;
template <typename T> using is_member_default_3way  = std::bool_constant
                             < member_default_3way<T> >;
}

// tupl<T...> size N pack specializations

#define TUPL_TYPE_ID XD
#define TUPL_DATA_ID xD
#define TYPENAME_DECL(n) typename TUPL_TYPE_ID(n)
#define TUPL_t_DATA_FWD(n) ((T&&)t).TUPL_DATA_ID(n)
#define MEMBER_DECL(n) TUPL_NUA TUPL_TYPE_ID(n) TUPL_DATA_ID(n);

#define TUPL_TYPE_IDS XREPEAT(VREPEAT_INDEX,TUPL_TYPE_ID,COMMA)
#define TUPL_DATA_IDS XREPEAT(VREPEAT_INDEX,TUPL_DATA_ID,COMMA)
#define TYPENAME_DECLS XREPEAT(VREPEAT_INDEX,TYPENAME_DECL,COMMA)
#define TUPL_t_DATA_FWDS XREPEAT(VREPEAT_INDEX,TUPL_t_DATA_FWD,COMMA)
#define MEMBER_DECLS XREPEAT(VREPEAT_INDEX,MEMBER_DECL,NOSEP)

#define MAP_V(...) template <same_ish<TUPL_ID> T> friend constexpr \
decltype(auto) map([[maybe_unused]]T&& t, auto f) \
noexcept(noexcept(f(__VA_ARGS__))) { return f(__VA_ARGS__); }

#define R_TUPL tupl_assign_fwd_t<TUPL_ID>
//#define TUPL_TIE_T tupl_tie_t<TUPL_ID>

#define TUPL_PASS 1
#define VREPEAT_COUNT TUPL_MAX_ARITY
#define VREPEAT_MACRO tupl_dev/tupl_impl_pre.hpp
#include "VREPEAT.hpp"

#undef TUPL_PASS
#define TUPL_PASS 2
#include __FILE__

#elif (TUPL_PASS == 1)

template <TYPENAME_DECLS>
struct TUPL_ID<TUPL_TYPE_IDS>
{
// using tie_t = TUPL_TIE_T const;
 using tupl_t = TUPL_ID;

 MEMBER_DECLS

 static constexpr std::integral_constant<std::size_t,NREPEAT> size{};

 friend auto operator<=>(TUPL_ID const&,TUPL_ID const&)
#if NREPEAT != 0
   requires types_all<TUPL_ID,impl::is_member_default_3way>
#endif
   = default;
#if NREPEAT != 0
// template<typename...>constexpr auto& operator=(std::true_type)
//   requires (tupl_val<TUPL_ID>&&types_all<TUPL_ID,assign_overload>)
//   {return assign_to<TUPL_ID>{*this} = {};}

 template<typename...>constexpr auto& operator=(R_TUPL r)
   noexcept(noexcept(assign_to{*this} = r))
   //requires (types_all<TUPL_ID,assign_overload>)
   {return assign_to{*this} = r;}

 template<typename...>constexpr auto& operator=(R_TUPL r) const
   noexcept(noexcept(assign_to{*this} = r))
   requires (types_all<TUPL_ID,is_const_assignable>)
   {return assign_to{*this} = r;}

 constexpr auto&operator=(std::same_as<tupl_tie_t<TUPL_ID>>auto const&r)
   noexcept(noexcept(assign_to{*this} = r))
   {return assign_to{*this} = r;}
#endif
 MAP_V(TUPL_t_DATA_FWDS)
};

#else // TUPL_PASS == 2

//
// get<I>(t)
//
template <std::size_t I, tuplish T>
  constexpr auto&& get(T&& t) noexcept
  requires (I < tupl_size<T>)
{
 using R = T&&;
#define ELSE() else
#define MACRO(N) if constexpr(I==HEXLIT(N))return((R)t).TUPL_DATA_ID(N);
 XREPEAT(TUPL_MAX_ARITY,MACRO,ELSE)
#undef MACRO
 UNREACHABLE();
}
//
// compare(l,r): 3-way comparison for tupls with <=> comparable elements
//
template <typename...L, typename...R>
  requires (three_way_comparable_with<L,R> && ...)
constexpr auto compare(TUPL_ID<L...> const& l, TUPL_ID<R...> const& r)
noexcept{
 constexpr auto s = sizeof...(L);
 constexpr compare_three_way cmp;
 if constexpr (s==0) return cmp(0,0);
#define MACRO(N) if constexpr(HEXLIT(N)<s){if(auto c=cmp(\
l.TUPL_DATA_ID(N),r.TUPL_DATA_ID(N));c!=0||HEXLIT(INC(N))==s)return c;}
 XREPEAT(TUPL_MAX_ARITY,MACRO,NOSEP)
#undef MACRO
 UNREACHABLE();
}
//
template <tuplish L>
constexpr auto compare(L const& l, tupl_assign_fwd_t<L> r) noexcept
  { return compare(l,r); }
//
// equals(l,r) == comparison for tupls with all == comparable elements
//
template <typename...L, typename...R>
  requires (equality_comparable_with<L,R> && ...)
constexpr bool equals(TUPL_ID<L...> const& l, TUPL_ID<R...> const& r)
noexcept{
  return map(l, [&r](L const&...lh) {
    return map(r, [&lh...](R const&...rh)
    {
      return ((lh == rh) && ...); // no short circuit, intended
    });
  });
}
//
template <tuplish L>
constexpr bool equals(L const& l, tupl_assign_fwd_t<L> r) noexcept
  { return equals(l,r); }
//
template <tuplish T>
  requires (! types_all<T,impl::is_member_default_3way>
           && types_all<T,impl::is_three_way_comparable>)
constexpr auto operator<=>(T const& l,T const& r) noexcept {
  return compare(l,r);
}

template <tuplish T>
  requires (! types_all<T,impl::is_member_default_3way>
           && types_all<T,impl::is_three_way_comparable>)
constexpr auto operator==(T const& l,T const& r) noexcept {
    return l <=> r == 0;
}
template <tuplish T>
  requires (! types_all<T,impl::is_member_default_3way>
         && ! types_all<T,impl::is_three_way_comparable>
           && types_all<T,impl::is_equality_comparable>)
constexpr bool operator==(T const& l,T const& r) noexcept {
  return equals(l,r);
}
//
template <std::size_t I, typename...E>
using type_pack_element = typename std::remove_cvref_t<decltype(
      get<I>(TUPL_ID<std::type_identity<E>...>{}))>::type;
//
// tupl_element<I,Tupl> == type_identity<tupl_element_t>{}
// tupl_element_t<I,Tupl> type of element I in Tupl
//
// (note: Tupl is instantiated, unlike std::tuple_element)
//
template <std::size_t I, typename T>
struct tupl_element;
//
template <std::size_t I, typename T>
using tupl_element_t = typename tupl_element<I,T>::type;
//
template <std::size_t I, typename...E>
struct tupl_element<I,TUPL_ID<E...>> {
  using type = type_pack_element<I,E...>;
};
//
// tupl_indexof<X,Tupl> Index of element of type X in Tupl
//
template <typename X, typename T> extern const int tupl_indexof;
//
template <typename X, typename...E>
inline constexpr int tupl_indexof<X,TUPL_ID<E...>> = []
{
  static_assert((std::same_as<X,E> + ...) == 1,
      "get<TYPE>(tupl) error: tupl should have a single TYPE element");
  int index = 0;
  int count = 0;
  ((std::same_as<X,E> ? index = count : ++count), ...);
  return index;
}();
//
// get<T>(t)
//
template <typename X, tuplish T>
constexpr auto&& get(T&& t) noexcept
{
  constexpr int index = tupl_indexof<X, std::remove_cvref_t<T>>;
  return get<index>((T&&)t);
}

// tie(a,b) -> tupl<decltype(a)&, decltype(b)&>{a,b}
//
template <typename...T>
constexpr auto tie(T&...t) noexcept
  -> TUPL_ID<decltype(t)...> const
{
    return { t... };
}

// getie<I...>(tupl) -> tie(get<I>(tupl)...)
//
template <unsigned...I, tuplish T>
constexpr auto getie(T&& t) noexcept
  -> TUPL_ID<decltype(get<I>((T&&)t))...> const
     requires ((I < tupl_size<T>) && ...)
    { return {get<I>((T&&)t)...}; }

template <typename T>
concept self_constructible = std::constructible_from<T,T>;

// dupl<I...>(tupl) -> tupl{get<I>(tupl)...};
//
template <unsigned...I, typename...E>
constexpr auto dupl(TUPL_ID<E...>const& a)
  -> TUPL_ID<type_pack_element<I,E...>...>
{
  if constexpr ((self_constructible<type_pack_element<I,E...>> && ...))
    return {get<I>(a)...};
  else
    return TUPL_ID<type_pack_element<I,E...>...>{} = {get<I>(a)...};
}

#include "IREPEAT_UNDEF.hpp"

#undef R_TUPL
#undef MAP_V

#undef MEMBER_DECLS
#undef TUPL__t_DATA_FWDS
#undef TYPENAME_DECLS
#undef TUPL_DATA_IDS
#undef TUPL_TYPE_IDS

#undef MEMBER_DECL
#undef TUPL_t_DATA_FWD
#undef TYPENAME_DECL
#undef TUPL_DATA_ID
#undef TUPL_TYPE_ID

#undef TUPL_NUA

#undef TUPL_MAX_ARITY_ID
#undef TUPL_MAX_ARITY

#include "namespace.hpp"

#undef NAMESPACE_ID
#undef TUPL_ID

#undef TUPL_PASS

#endif
