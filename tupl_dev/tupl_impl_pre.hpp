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

#include "namespace.hpp" // Optional namespace, defaults to lml
//
inline constexpr size_t tupl_max_arity = HEXLIT(TUPL_MAX_ARITY);
//
// tupl primary template declaration, tupl<E...>
//
template <typename...E> struct TUPL_ID;
//
// CTAD deduce values including arrays (no decay, unlike std tuple)
//
template <typename...E> TUPL_ID(E const&...) -> TUPL_ID<E...>;
//
namespace impl {
//
// tupl_cons adds constructors for tupl array members from array lvalues
//
template <typename...E> struct tupl_cons; // : tupl<E...>
//
template <typename...E> tupl_cons(E const&...) -> tupl_cons<E...>;
//
} // impl
//
template <typename...E> TUPL_ID(impl::tupl_cons<E...>) -> TUPL_ID<E...>;
//
// tuplish concept, requires T is a complete type with a member alias
//  'tupl_t' to which T is implicitly convertible (e.g. by inheritance).
//  Note that tuplish matches tupl with cvref qualifiers
//
//template <typename T> concept tuplish = std::is_convertible_v<
//     std::remove_cvref_t<T>, typename std::remove_cvref_t<T>::tupl_t>;
template <typename T> // Alternative definition (is_convertible fail
concept tuplish       // on clang & stdlibc++ declval in unevaluated)
 = requires (T v, typename std::remove_cvref_t<T>::tupl_t& u)
   {decltype(u)(v);}; // reference initialization implicit conversions//
// tupl_size<Tupl> number of tupl elements
//  (requires complete type to access tupl_t typdef)
//
template <tuplish Tupl> inline constexpr auto tupl_size
  = type_list_size<typename std::remove_cvref_t<Tupl>::tupl_t>;
//
// size(tupl) the number of tupl elements
//
template <tuplish T> constexpr auto size(T const&) noexcept
  { return tupl_size<T>; }

/*
  assign_fwd<U> 'forwarding' carrier type for tie assignment operator=
   -> void       for a non-assignable type (renders it incomplete)
           else V = remove_cvref_t<U> and:
   -> V          for trivially_copyable non-array V of 16 bytes or less
   -> V&&        if U is rvalue_ref (expects an rvalue move candidate)
   -> V const&   else for all other; big or non-trival V, lvalue
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
    if constexpr (std::is_rvalue_reference_v<U>)
        return std::type_identity<U>{};
    else
        return std::type_identity<V const&>{};
  }
  else
    return std::type_identity<void>{};
}
}
template <typename U>
struct assign_fwd : decltype(impl::assign_fwd_f<U>()) {};
//

// tupl_val<T> concept: tuplish type with all elements object-type
// and non const-assignable (which implies a proxy reference)
// (note: is_object matches unbounded array T[], allows for FAM)
//
template <typename Tupl>
concept tupl_val
      = tuplish<Tupl>
     && types_all<std::remove_cvref_t<Tupl>, impl::is_value>;
//
// tupl_tie<T> concept: matches tupl of reference-like elements detected
// as const-assignable; either reference-to-object assignable elements
// or assignable proxy-references.
// (note: const_assignable matches unbounded array reference T(&)[])
//
template <typename T>
concept tupl_tie
      = tuplish<T>
     && tupl_size<T> != 0
     && types_all<std::remove_cvref_t<T>, is_const_assignable>;
//
//template <tupl_val Tupl>
template <typename Tupl>
using tupl_tie_t =
         typename type_map<std::add_lvalue_reference_t,Tupl>::type;
// assignability traits for tupl extending array_assign
//
template <typename L, typename R, typename...>
inline constexpr bool tupls_assignable
                    = tupls_assignable<L,R,
                      std::remove_cvref_t<L>, std::remove_cvref_t<R>>;
//
template <typename L, typename R, typename...l, typename...r>
inline constexpr bool tupls_assignable<L,R,TUPL_ID<l...>,TUPL_ID<r...>>
          = (assignable_from<apply_cv_t<L,l>&, copy_ref_t<R,r>> && ...);
//
template <typename l, typename r, typename R>
inline constexpr bool noexcept_assignable = requires (l& h, r& s)
   {requires noexcept(assign(h,(copy_ref_t<R,r>)s));};
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
  using L = std::remove_reference_t<Lr>;

  using value_type = std::conditional_t<tupl_val<L>, L, const L>;

  value_type& l;

  template <tuplish R>
    requires (tupl_size<R> == tupl_size<L> && tupls_assignable<L, R&&>)
  constexpr value_type& operator=(R&& r) const noexcept(
    noexcept(tupls_noexcept_assignable<L,R&&>))
  {
    if constexpr (std::is_const_v<L>)
      [&]<typename...U>(TUPL_ID<U...>const&){
        map(l, [&](auto&...t) {
          map(r, [&](auto&...u) {
            (assign(t,(U)u), ...);
          });
        });
      }(r);
    else
      map(l, [&r](auto&...t) {
        map(r, [&t...](auto&&...u) {
          if constexpr (std::is_lvalue_reference_v<R>)
            (assign(t,u), ...);
          else
            (assign(t,(std::remove_reference_t<decltype(u)>&&)(u)),...);
        });
      });
    return l;
  }

  constexpr value_type& operator=(TUPL_ID<>const&) const noexcept
  {
    map(l, [](auto&...t) {(assign(t,{}),...);});
    return l;
  }
};
//
// swap(ta,tb) tupl swap specialization, including const-assignable
//
template <tuplish L>
constexpr void swap(L&& l, L&& r)
  noexcept(types_all<std::remove_cvref_t<L>, std::is_nothrow_swappable>)
  requires(types_all<std::remove_cvref_t<L>, std::is_swappable>)
{
  map(l, [&r](auto&...t) {
    map(r, [&t...](decltype(t)...u)
    {
      (std::ranges::swap((apply_ref_t<L&&,decltype(t)>)t,
                         (apply_ref_t<L&&,decltype(t)>)u), ...);
    });
  });
}

namespace impl {
//
template <typename T>
struct lref
{
  T const& r;
  constexpr lref(T& a) noexcept : r{a} {}
  constexpr lref(std::same_as<T const> auto& a) noexcept : r{a} {}
};
template <typename T>
struct rref
{
  T& r;
  constexpr rref(std::same_as<T> auto&& a) noexcept : r{a} {}
};
//
// tupl_cons adds constructors for tupl array members from array lvalues
// the client constrains to use this only if array members are present
//
template <typename...E>
struct tupl_cons : TUPL_ID<E...>
{
  using base = TUPL_ID<E...>;

  explicit (!(default_list_initializable<E> && ...))
  constexpr tupl_cons() = default;

  // Single array element constructor
  // Construct from ref-wrapped E to force braced copy list-init syntax,
  // as a single operator=(auto&&) forwarding arg works without braces
  //
  constexpr tupl_cons(lref<E>...e)
    noexcept(noexcept((assign(std::declval<E&>(),e.r),...)))
    requires (sizeof...(E) == 1)
  {
    assign(this->x0, (e.r,...));
  }
  constexpr tupl_cons(rref<E>...e)
    noexcept(noexcept((assign(std::declval<E&>(),(E&&)e.r),...)))
    requires (sizeof...(E) == 1)
  {
    assign(this->x0, ((E&&)e.r,...));
  }

  // Multi element constructor, dealing with array(s)
  //
  template <same_ish<E>...U>
  constexpr tupl_cons(U&&...e)
    noexcept(noexcept((assign(std::declval<E&>(),(U&&)e),...)))
    requires (sizeof...(U) > 1)
  {
    map(*(base*)this, [&](E&...l){ (assign(l,(U&&)e),...);} );
  }
};
} // impl
//
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
#define C_TUPL impl::tupl_cons<TUPL_TYPE_IDS>

#define TUPL_PASS 1
#define VREPEAT_COUNT TUPL_MAX_ARITY
#define VREPEAT_MACRO tupl_dev/tupl_impl_pre.hpp
#include "VREPEAT.hpp"

#undef TUPL_PASS
#define TUPL_PASS 2
#include __FILE__

#elif (TUPL_PASS == 1)
//
template <TYPENAME_DECLS>
struct TUPL_ID<TUPL_TYPE_IDS>
{
 using tupl_t = TUPL_ID;
MEMBER_DECLS
//
 friend auto operator<=>(TUPL_ID const&,TUPL_ID const&)
#if NREPEAT != 0
   requires types_all<TUPL_ID,is_member_default_3way>
#endif
   = default;
#if NREPEAT != 0
//
 template<typename...>constexpr auto& operator=(C_TUPL r)
   noexcept(noexcept(*this = static_cast<TUPL_ID&&>(r)))
   requires ( types_all<TUPL_ID, impl::is_value>
         && ! types_all<TUPL_ID, impl::is_direct_initialized>)
   {return *this = static_cast<TUPL_ID&&>(r);}
//
 template<typename...>constexpr auto& operator=(R_TUPL r) const
   noexcept(noexcept(assign_to{*this} = r))
   requires (types_all<TUPL_ID, is_const_assignable>)
   {return assign_to{*this} = r;}
//
 template<typename T> constexpr auto& operator=(T&& r) const
   noexcept(noexcept(assign_to{*this} = r))
     requires (tupl_size<T> == tupl_size<TUPL_ID>
           &&  tupls_assignable<TUPL_ID const,std::remove_cvref_t<T>>)
   {return assign_to{*this} = r;}
//
 template<typename...>constexpr auto& operator=(TUPL_ID<>) const
   requires (types_all<TUPL_ID, is_const_assignable>)
   {return assign_to{*this} = TUPL_ID<>{};}
#endif
 MAP_V(TUPL_t_DATA_FWDS)
};

#else // TUPL_PASS == 2

//
// get<I>(t)
//
template <size_t I, tuplish T>
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
// operator<=> overload, when possible and not defaulted
//
template <tuplish T>
  requires (! types_all<T, is_member_default_3way>
           && types_all<T, is_three_way_comparable>)
constexpr auto operator<=>(T const& l,T const& r) noexcept {
  return compare(l,r);
}
//
// operator== overloads, when possible and not defaulted
// (Test that these overload are not ambiguous)
//
template <tuplish T>
  requires (! types_all<T, is_member_default_3way>
           && types_all<T, is_three_way_comparable>)
constexpr auto operator==(T const& l,T const& r) noexcept {
    return l <=> r == 0;
}
//
template <tuplish T>
  requires (! types_all<T, is_member_default_3way>
         && ! types_all<T, is_three_way_comparable>
           && types_all<T, is_equality_comparable>)
constexpr bool operator==(T const& l,T const& r) noexcept {
  return equals(l,r);
}
//

// tupl_element<I,Tupl> == type_identity<tupl_element_t>{}
// tupl_element_t<I,Tupl> type of element I in Tupl
//
template <size_t I, typename T>
struct tupl_element;
//
template <size_t I, typename T>
using tupl_element_t = typename tupl_element<I,T>::type;
//
template <size_t I, typename...E>
struct tupl_element<I,TUPL_ID<E...>> {
  using type = type_pack_element<I,E...>;
};
//
// tupl_indexof<X,Tupl> Index of element of type X in Tupl
//
template <typename X, typename T> extern const int tupl_indexof;
//
template <typename X, typename...E>
inline constexpr int tupl_indexof<X,TUPL_ID<E...>>
              = type_pack_indexof<X,E...>;
//
// get<T>(t)
//
template <typename X, tuplish T>
constexpr auto&& get(T&& t) noexcept
{
  return get<tupl_indexof<X, std::remove_cvref_t<T>>>((T&&)t);
}
//
// tie(a,b) -> tupl<decltype(a)&, decltype(b)&>{a,b}
//
template <typename...T>
constexpr auto tie(T&...t) noexcept
  -> TUPL_ID<T&...> const
{
    return { t... };
}
//
template <typename...T>
constexpr auto tie_fwd(T&&...t) noexcept
  -> TUPL_ID<T&&...> const
{
    return { (T&&)t... };
}
//
// getie<I...>(tupl) -> tie(get<I>(tupl)...)
//
template <unsigned...I, tuplish T>
constexpr auto getie(T&& t) noexcept
  -> TUPL_ID<decltype(get<I>((T&&)t))...> const
     requires ((I < tupl_size<T>) && ...)
    { return {get<I>((T&&)t)...}; }

template <typename T>
concept self_constructible = std::constructible_from<T,T>;
//
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
//
// tupl_cat(tupl_a,tupl_b) concantenate two tupls
//
template <tuplish T, tuplish U>
constexpr auto tupl_cat(T&& t, U&& u)
{
  return map(t,[&u]<typename...Ts>(Ts const&...ts){
    return map(u,[&ts...]<typename...Us>(Us const&...us)
      -> tupl<Ts...,Us...>
    {
      if constexpr ((self_constructible<Ts> && ...)
                 && (self_constructible<Us> && ...))
        return{(copy_cvref_t<T&&,Ts>)ts...,(copy_cvref_t<U&&,Us>)us...};
      else
        return tupl<Ts...,Us...>{} // requires all default construtible
            = {(copy_cvref_t<T&&,Ts>)ts...,(copy_cvref_t<U&&,Us>)us...};
    });
  });
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
