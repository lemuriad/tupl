#ifndef TUPL_PASS    // Three conditional sections, empty,1,2

#ifndef TUPL_ID      // TUPL_ID = your name for the tuple template id
#define TUPL_ID tupl // E.g. configure with -DTUPL_ID=tuple
#endif

#ifndef TUPL_MAX_INDEX // TUPL_MAX_INDEX = tupl max 'arity' minus one
#define TUPL_MAX_INDEX (f) // specified as parenthesized hex digits
#endif

#ifndef TUPL_NUA // TUPL_NUA : define empty to disable no_unique_address
#define TUPL_NUA [[no_unique_address]]
#endif

#if __has_include("tupl_impl_noedit_warn.hpp") // Warning banner for the
#include "tupl_impl_noedit_warn.hpp" // generated file; Don't edit this!
#endif

#include "IREPEAT.hpp"

#define TUPL_MAX_ARITY CAT(TUPL_ID,_max_arity) // tupl_max_arity value

#include "namespace.hpp" // Optional namespace, defaults to lml

template <typename T> using DEFAULT_ASSIGNABLE = std::bool_constant<
                            default_assignable<T>>;
template <typename T> using THREE_WAY_COMPARABLE = std::bool_constant<
                            three_way_comparable<T>>;
template <typename T> using EQUALITY_COMPARABLE  = std::bool_constant<
                            equality_comparable<T>>;
template <typename T> using MEMBER_DEFAULT_3WAY  = std::bool_constant<
                            member_default_3way<T>>;

template <typename T> using ASSIGN_OVERLOAD = std::bool_constant<
                            assignable_from<T&,T&>
                  && ! std::assignable_from<T&,T&>>;

inline constexpr std::size_t TUPL_MAX_ARITY = HEXLIT(TUPL_MAX_INDEX)+1;

//
// Primary template declaration, requires sizeof...(T) < TUPL_MAX_ARITY
template <typename...T> struct TUPL_ID;
//

namespace impl {
// decay_f<T> decay function type (hard fail for abominable functions)
template <typename T, typename U = std::remove_cvref_t<T>>
using decay_f = std::conditional_t<std::is_function_v<U>,U*,U>;
}
// CTAD deduce values including arrays (i.e. no decay, unlike std tuple)
template <typename...E>
TUPL_ID(E&&...) -> TUPL_ID<impl::decay_f<E>...>;
// CTAD deduce first argument as an rvalue array from braced init-list
template <typename T, int N, typename...E>
TUPL_ID(T(&&)[N], E&&...) -> tupl<T[N], impl::decay_f<E>...>;
//
//template<class T,class...A> auto to_tupl(T<A...>const&) -> TUPL_ID<A...>;
//template<class T> using to_tupl = decltype(as_tupl(std::declval<T>()));
// is_tupl trait
//template<class T> inline constexpr bool is_tupl = false;
//template<class...A> inline constexpr bool is_tupl<TUPL_ID<A...>> = true;
//
// tuplish concept, requires a 'tupl_t' member alias
template <typename T> concept tuplish = std::is_convertible_v<T,
          typename std::remove_cvref_t<T>::tupl_t>;

// tupl_size variable template (unrelated to std::tuple_size)
template <tuplish T> extern const std::size_t tupl_size;
template <typename...U> inline constexpr auto tupl_size<TUPL_ID<U...>>
         = sizeof...(U);

// size<Tupl>(): the number of tupl elements as an integral constant
template <tuplish T> constexpr auto size()
-> std::integral_constant<std::size_t,tupl_size<std::remove_cvref_t<T>>>
{ return {}; }
// size(tupl): the number of tupl elements as an integral constant
template <tuplish T> constexpr auto size(T const&)
-> std::integral_constant<std::size_t,tupl_size<std::remove_cvref_t<T>>>
{ return {}; }

// tupl_types_all meta function
template <typename, template<typename>class P>
inline constexpr bool tupl_types_all = false;
//
template <typename...T, template<typename>class P>
inline constexpr bool tupl_types_all<TUPL_ID<T...>,P> = (P<T>() && ...);
//

// tupl_val concept: tuplish type with all object-type elements
// (note: is_object matches unbounded array T[], allows for FAM)
template <typename T>
concept tupl_val = tuplish<T>
     && tupl_types_all<std::remove_cvref_t<T>, std::is_object>;

template <typename T> using is_object_lval_ref =
   std::bool_constant< std::is_lvalue_reference<T>()
                    && std::is_object<std::remove_cvref_t<T>>()>;

// tupl_tie concept: tuplish type with all reference-to-object elements
// (note: is_object matches unbounded array T[] so T(&)[] is admitted)
template <typename T>
concept tupl_tie =
        tuplish<T>
     && ! std::same_as<std::remove_cvref_t<T>, tupl<>>
     && tupl_types_all<std::remove_cvref_t<T>, is_object_lval_ref>;

// tupl_type_map<tupl<T...>, map> -> tupl<map<T>...>
template <template<typename...>class, typename>
struct tupl_type_map;

template <template<typename...>class M, typename...T>
struct tupl_type_map<M, TUPL_ID<T...>> {
    using type = TUPL_ID<M<T>...>;
};
//

namespace impl {
template <typename U>
auto assign_fwd_f() {
  using V = std::remove_cvref_t<U>;
  if constexpr (std::is_move_assignable_v<std::remove_all_extents_t<V>>)
  {
    if constexpr (std::is_trivially_copyable_v<V> && !std::is_array_v<V>
                  && sizeof(V)<=16)
        return std::type_identity<V>{};
    else
    if constexpr (std::is_lvalue_reference_v<U> || std::is_array_v<V>)
        return std::type_identity<V const&>{};
    else
        return std::type_identity<V&&>{};
  }
  else return std::type_identity<void>{};
}
}
// assign_fwd<U> 'forwarding' carrier type for assignment operator=
//  -> void       for a non-assignable type (renders it incomplete)
//          else V = remove_cvref_t<U> and:
//  -> V          for trivially_copyable non-array V of 16 bytes or less
//  -> V const&   for lvalue_ref U or array V (no move for & or array)
//  -> V&&        otherwise
//    (V&& won't accept V&, so either move or const_cast to V const&)
template <typename U>
using assign_fwd = typename decltype(impl::assign_fwd_f<U>())::type;
//

//
template <typename Tupl>
using tupl_tie_t =
         typename tupl_type_map<std::add_lvalue_reference_t,Tupl>::type;
//
template <typename Tupl>
using tupl_assign_fwd_t = typename tupl_type_map<assign_fwd,Tupl>::type;
//

// tupl<> zero-size pack specialization
template <> // tupl<T...> specializations defined in TUPL_PASS == 1
struct TUPL_ID<>
{
//  using tie_t = TUPL_ID;
  using tupl_t = TUPL_ID;

  static consteval std::size_t size() noexcept { return {}; }

  auto operator<=>(TUPL_ID const&) const = default;

  constexpr decltype(auto) operator()(auto f) const
     noexcept(noexcept(f())) { return f(); }
};
//

template <typename L, typename R>
inline constexpr bool tupls_assignable {};

template <typename...U, typename...V>
inline constexpr bool tupls_assignable<TUPL_ID<U...>,TUPL_ID<V...>> =
                           (assignable_from<U&,V> && ...);

// customization for tuple-tuple assignment
template <tuplish Lr>
struct assign_to<Lr>
{
  using L = std::remove_cvref_t<Lr>;

  using value_type = std::conditional_t<tupl_val<L>, L, const L>;

  value_type& l;

  template <tuplish R>
    requires (tupls_assignable<L, std::remove_cvref_t<R>>)
  constexpr value_type& operator=(R&& r) const noexcept(
    std::is_reference_v<R>
    ? tupl_types_all<R, std::is_nothrow_copy_assignable> // array R?
    : tupl_types_all<R, std::is_nothrow_move_assignable>)// array R?
  {
      map(l, [&r](auto&...t) {
          map(r, [&t...](auto&...u) {
            if constexpr (std::is_reference_v<R>)
              (assign(t,u), ...);
            else
              (assign(t,mv(u)), ...);
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
        map(r, [&t...](T&...u) {
            (std::ranges::swap(t, u), ...);
        });
    });
}

// tupl<T...> size N pack specializations

#define TUPL_TYPE_ID XD
#define TUPL_DATA_ID xD
#define TYPENAME_DECL(n) typename TUPL_TYPE_ID(n)
#define TUPL_t_DATA_FWD(n) ((T&&)t).TUPL_DATA_ID(n)
#define MEMBER_DECL(n) TUPL_NUA TUPL_TYPE_ID(n) TUPL_DATA_ID(n);

#define TUPL_TYPE_IDS IREPEAT(VREPEAT_INDEX,TUPL_TYPE_ID,COMMA)
#define TUPL_DATA_IDS IREPEAT(VREPEAT_INDEX,TUPL_DATA_ID,COMMA)
#define TYPENAME_DECLS IREPEAT(VREPEAT_INDEX,TYPENAME_DECL,COMMA)
#define TUPL_t_DATA_FWDS IREPEAT(VREPEAT_INDEX,TUPL_t_DATA_FWD,COMMA)
#define MEMBER_DECLS IREPEAT(VREPEAT_INDEX,MEMBER_DECL,NOSEP)

#define MAP_V(...) template <same_ish<TUPL_ID> T> friend constexpr \
decltype(auto) map(T&& t, auto f) noexcept(noexcept(f(__VA_ARGS__)))\
 { return f(__VA_ARGS__); }

#define R_TUPL tupl_assign_fwd_t<TUPL_ID>
//#define TUPL_TIE_T tupl_tie_t<TUPL_ID>

#define TUPL_PASS 1
#define VREPEAT_COUNT TUPL_MAX_INDEX
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

 static consteval auto size() noexcept { return NREPEAT+1; }

 friend auto operator<=>(TUPL_ID const&,TUPL_ID const&)
   requires tupl_types_all<TUPL_ID,MEMBER_DEFAULT_3WAY> = default;

 template<typename...>constexpr auto& operator=(TUPL_ID<>)
   requires (tupl_val<TUPL_ID>&&tupl_types_all<TUPL_ID,ASSIGN_OVERLOAD>)
   {return assign_to<TUPL_ID>{*this} = {};}

 template<typename...>constexpr auto& operator=(R_TUPL r)
   requires (tupl_val<TUPL_ID>&&tupl_types_all<TUPL_ID,ASSIGN_OVERLOAD>)
   {return assign_to<TUPL_ID>{*this} = r;}

 template<typename...>constexpr auto& operator=(R_TUPL r) const
   requires tupl_tie<TUPL_ID>
   {return assign_to<TUPL_ID>{*this} = r;}

 constexpr auto&operator=(std::same_as<tupl_tie_t<TUPL_ID>>auto const&r)
   {return assign_to<TUPL_ID>{*this} = r;}

 MAP_V(TUPL_t_DATA_FWDS)
};

#else

template <tuplish T> requires (! tupl_types_all<T,MEMBER_DEFAULT_3WAY>
                              && tupl_types_all<T,THREE_WAY_COMPARABLE>)
constexpr auto operator<=>(T const& l,T const& r) noexcept {
 constexpr auto s = tupl_size<T>;
 constexpr compare_three_way cmp;
#define MACRO(N) if constexpr(HEXLIT(N)<s){if(auto c=cmp(\
l.TUPL_DATA_ID(N),r.TUPL_DATA_ID(N));c!=0||1+HEXLIT(N)==s)return c;}
 IREPEAT(TUPL_MAX_INDEX,MACRO,NOSEP)
#undef MACRO
 UNREACHABLE();
}

template <tuplish T> requires (! tupl_types_all<T,MEMBER_DEFAULT_3WAY>
                              && tupl_types_all<T,THREE_WAY_COMPARABLE>)
constexpr auto operator==(T const& l,T const& r) noexcept {
    return l <=> r == 0;
}
template <tuplish T> requires (! tupl_types_all<T,MEMBER_DEFAULT_3WAY>
                            && ! tupl_types_all<T,THREE_WAY_COMPARABLE>
                              && tupl_types_all<T,EQUALITY_COMPARABLE>)
constexpr bool operator==(T const& l,T const& r) noexcept {
    constexpr auto s = tupl_size<T>;
#define MACRO(N) if constexpr(HEXLIT(N)<s){if(bool c=\
l.TUPL_DATA_ID(N)==r.TUPL_DATA_ID(N);!c||1+HEXLIT(N)==s)return c;}
 IREPEAT(TUPL_MAX_INDEX,MACRO,NOSEP)
#undef MACRO
 UNREACHABLE();
}

// get<I>(t)
template <int I> constexpr auto&& get(tuplish auto&& t) noexcept
  requires (I < t.size())
{
 using T = decltype(t);
#define ELSE() else
#define MACRO(N) if constexpr(I==HEXLIT(N))return((T)t).TUPL_DATA_ID(N);
 IREPEAT(TUPL_MAX_INDEX,MACRO,ELSE)
#undef MACRO
 UNREACHABLE();
}

// Index of first element of type X
template <typename X, typename...A> requires (std::same_as<X,A> || ...)
inline constexpr int indexof = []() consteval {
 using std::same_as;
 using T = TUPL_ID<A...>;
#define TUPL_TYPE(N) decltype(T::TUPL_DATA_ID(N))
#define MACRO(N) if constexpr(same_as<X,TUPL_TYPE(N)>)return HEXLIT(N);
 IREPEAT(TUPL_MAX_INDEX,MACRO,ELSE)
#undef MACRO
#undef TUPL_TYPE
#undef ELSE
 UNREACHABLE();
}();
//
template <typename X, typename T>
inline constexpr int tupl_indexof = not defined(tupl_indexof<X,T>);
template <typename X, typename...A>
inline constexpr int tupl_indexof<X,TUPL_ID<A...>> = indexof<X,A...>;

template <typename...T>
constexpr auto tie(T&...t) noexcept
  -> TUPL_ID<decltype(t)...> const
{
    return { t... };
};

template <int...I>
constexpr auto getie(tuplish auto&& t) noexcept
  -> TUPL_ID<decltype(get<I>((decltype(t))t))...> const
     requires ((I < t.size()) && ...)
    { return {get<I>((decltype(t))t)...}; };

template <auto...A, typename...E>
constexpr auto dupl(TUPL_ID<E...>const& a)
  -> TUPL_ID<decltype(+get<A>(a))...>
    { return {get<A>(a)...}; };

template <typename...A, typename...E>
constexpr auto dupl(TUPL_ID<E...>const& a) -> TUPL_ID<A...>
    { return {get<A>(a)...}; };

#include "IREPEAT_UNDEF.hpp"

#undef R_TUPL
#undef MAP_V

#undef MEMBER_DECLS
#undef TUPL_DATA_FWDS
#undef TYPENAME_DECLS
#undef TUPL_DATA_IDS
#undef TUPL_TYPE_IDS

#undef MEMBER_DECL
#undef TUPL_DATA_FWD
#undef TYPENAME_DECL
#undef TUPL_DATA_ID
#undef TUPL_TYPE_ID

#undef TUPL_MAX_INDEX
#undef TUPL_NUA

#undef TUPL_MAX_ARITY
#undef TUPL_CONSTRUCT
#undef TUPL_MAKE

#include "namespace.hpp"

#undef NAMESPACE_ID
#undef TUPL_ID

#undef TUPL_PASS

#endif
