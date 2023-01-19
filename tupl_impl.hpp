/*
     Copyright (c) 2022 Lemurian Labs https://lemurianlabs.com/

     Distributed under the Boost Software License, Version 1.0.
           http://www.boost.org/LICENSE_1_0.txt

     Repo: https://github.com/Lemurian-Labs/tupl
*/
/* WARNING: Do not edit this file "tupl_impl.hpp"
   =======
* it's generated by preprocessing "tupl_impl_pre.hpp" so is overwritten
* each time the preprocessing step is rerun, very likely automatically.
***********************************************************************/
//
inline constexpr std::size_t tupl_max_arity = 0x10;
/*
   tupl<E...> primary template declaration; an aggregate struct
   with [[no_unique_address]] attribute on all element member decls E...
*/
template <typename...E> struct tupl; // aggregate [[no_unique_address]]
//
// tupl CTAD deduces elements by-value, including arrays (and functions)
//
template <typename...E> tupl(E const&...) -> tupl<E...>;
//
template<typename T> inline constexpr bool is_tupl_v = false;
template<typename...E>inline constexpr bool is_tupl_v<tupl<E...>>{true};
//
template<typename T> inline constexpr bool is_lupl_v = false;
/*
   lupl<E...> 'layout compatible' tupl without [[no_unique_address]]
*/
template <typename...E> struct lupl; // aggregate, layout-compatibility
template <typename...E> lupl(E const&...) -> lupl<E...>;
template<typename...E>inline constexpr bool is_lupl_v<lupl<E...>>{true};
//
template<typename T>
concept tupl_or_lupl = is_tupl_v<tupl_t<T>> || is_lupl_v<tupl_t<T>>;
/*
   Derived tuplish types with custom CTAD only, nothing else added
*/
template <typename...E> struct fwds : tupl<E...> {}; // forwarding refs
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
//
// tupl<T...> specializations VREPEATed for each arity
//
template <>
struct tupl<>
{
 using tupl_t = tupl;
 //

 //
 friend auto operator<=>(tupl const&,tupl const&)
  = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map([[maybe_unused]]T&& t, F f)noexcept(
  noexcept(f()))
  { return f(); }
};
//
template <class X0>
struct tupl<X0>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0)))
  { return f(U(t).x0); }
};
//
template <class X0,class X1>
struct tupl<X0,X1>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1)))
  { return f(U(t).x0,U(t).x1); }
};
//
template <class X0,class X1,class X2>
struct tupl<X0,X1,X2>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2)))
  { return f(U(t).x0,U(t).x1,U(t).x2); }
};
//
template <class X0,class X1,class X2,class X3>
struct tupl<X0,X1,X2,X3>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3); }
};
//
template <class X0,class X1,class X2,class X3,class X4>
struct tupl<X0,X1,X2,X3,X4>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5>
struct tupl<X0,X1,X2,X3,X4,X5>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;NUA X5 x5;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6>
struct tupl<X0,X1,X2,X3,X4,X5,X6>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;NUA X5 x5;NUA X6 x6;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7>
struct tupl<X0,X1,X2,X3,X4,X5,X6,X7>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;NUA X5 x5;NUA X6 x6;NUA X7 x7;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8>
struct tupl<X0,X1,X2,X3,X4,X5,X6,X7,X8>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;NUA X5 x5;NUA X6 x6;NUA X7 x7;NUA X8 x8;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9>
struct tupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;NUA X5 x5;NUA X6 x6;NUA X7 x7;NUA X8 x8;NUA X9 x9;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa>
struct tupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;NUA X5 x5;NUA X6 x6;NUA X7 x7;NUA X8 x8;NUA X9 x9;NUA Xa xa;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa,class Xb>
struct tupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa,Xb>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;NUA X5 x5;NUA X6 x6;NUA X7 x7;NUA X8 x8;NUA X9 x9;NUA Xa xa;NUA Xb xb;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa,class Xb,class Xc>
struct tupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa,Xb,Xc>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;NUA X5 x5;NUA X6 x6;NUA X7 x7;NUA X8 x8;NUA X9 x9;NUA Xa xa;NUA Xb xb;NUA Xc xc;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa,class Xb,class Xc,class Xd>
struct tupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa,Xb,Xc,Xd>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;NUA X5 x5;NUA X6 x6;NUA X7 x7;NUA X8 x8;NUA X9 x9;NUA Xa xa;NUA Xb xb;NUA Xc xc;NUA Xd xd;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa,class Xb,class Xc,class Xd,class Xe>
struct tupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa,Xb,Xc,Xd,Xe>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;NUA X5 x5;NUA X6 x6;NUA X7 x7;NUA X8 x8;NUA X9 x9;NUA Xa xa;NUA Xb xb;NUA Xc xc;NUA Xd xd;NUA Xe xe;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd,U(t).xe)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd,U(t).xe); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa,class Xb,class Xc,class Xd,class Xe,class Xf>
struct tupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa,Xb,Xc,Xd,Xe,Xf>
{
 using tupl_t = tupl;
 //
 NUA X0 x0;NUA X1 x1;NUA X2 x2;NUA X3 x3;NUA X4 x4;NUA X5 x5;NUA X6 x6;NUA X7 x7;NUA X8 x8;NUA X9 x9;NUA Xa xa;NUA Xb xb;NUA Xc xc;NUA Xd xd;NUA Xe xe;NUA Xf xf;
 //
 friend auto operator<=>(tupl const&,tupl const&)
  requires types_all<is_member_default_3way,tupl> = default;
 //
 template <same_ish<tupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd,U(t).xe,U(t).xf)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd,U(t).xe,U(t).xf); }
};
//
// lupl<T...> specializations VREPEATed for each arity
//
template <>
struct lupl<>
{
 using tupl_t = lupl;
 //

 //
 friend auto operator<=>(lupl const&,lupl const&)
  = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map([[maybe_unused]]T&& t, F f)noexcept(
  noexcept(f()))
  { return f(); }
};
//
template <class X0>
struct lupl<X0>
{
 using tupl_t = lupl;
 //
 X0 x0;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0)))
  { return f(U(t).x0); }
};
//
template <class X0,class X1>
struct lupl<X0,X1>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1)))
  { return f(U(t).x0,U(t).x1); }
};
//
template <class X0,class X1,class X2>
struct lupl<X0,X1,X2>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2)))
  { return f(U(t).x0,U(t).x1,U(t).x2); }
};
//
template <class X0,class X1,class X2,class X3>
struct lupl<X0,X1,X2,X3>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3); }
};
//
template <class X0,class X1,class X2,class X3,class X4>
struct lupl<X0,X1,X2,X3,X4>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5>
struct lupl<X0,X1,X2,X3,X4,X5>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;X5 x5;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6>
struct lupl<X0,X1,X2,X3,X4,X5,X6>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;X5 x5;X6 x6;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7>
struct lupl<X0,X1,X2,X3,X4,X5,X6,X7>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;X5 x5;X6 x6;X7 x7;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8>
struct lupl<X0,X1,X2,X3,X4,X5,X6,X7,X8>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;X5 x5;X6 x6;X7 x7;X8 x8;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9>
struct lupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;X5 x5;X6 x6;X7 x7;X8 x8;X9 x9;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa>
struct lupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;X5 x5;X6 x6;X7 x7;X8 x8;X9 x9;Xa xa;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa,class Xb>
struct lupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa,Xb>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;X5 x5;X6 x6;X7 x7;X8 x8;X9 x9;Xa xa;Xb xb;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa,class Xb,class Xc>
struct lupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa,Xb,Xc>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;X5 x5;X6 x6;X7 x7;X8 x8;X9 x9;Xa xa;Xb xb;Xc xc;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa,class Xb,class Xc,class Xd>
struct lupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa,Xb,Xc,Xd>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;X5 x5;X6 x6;X7 x7;X8 x8;X9 x9;Xa xa;Xb xb;Xc xc;Xd xd;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa,class Xb,class Xc,class Xd,class Xe>
struct lupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa,Xb,Xc,Xd,Xe>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;X5 x5;X6 x6;X7 x7;X8 x8;X9 x9;Xa xa;Xb xb;Xc xc;Xd xd;Xe xe;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd,U(t).xe)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd,U(t).xe); }
};
//
template <class X0,class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8,class X9,class Xa,class Xb,class Xc,class Xd,class Xe,class Xf>
struct lupl<X0,X1,X2,X3,X4,X5,X6,X7,X8,X9,Xa,Xb,Xc,Xd,Xe,Xf>
{
 using tupl_t = lupl;
 //
 X0 x0;X1 x1;X2 x2;X3 x3;X4 x4;X5 x5;X6 x6;X7 x7;X8 x8;X9 x9;Xa xa;Xb xb;Xc xc;Xd xd;Xe xe;Xf xf;
 //
 friend auto operator<=>(lupl const&,lupl const&)
  requires types_all<is_member_default_3way,lupl> = default;
 //
 template <same_ish<lupl> T, typename F, typename U = T&&>
 friend constexpr decltype(auto) map(T&& t, F f)noexcept(
  noexcept(f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd,U(t).xe,U(t).xf)))
  { return f(U(t).x0,U(t).x1,U(t).x2,U(t).x3,U(t).x4,U(t).x5,U(t).x6,U(t).x7,U(t).x8,U(t).x9,U(t).xa,U(t).xb,U(t).xc,U(t).xd,U(t).xe,U(t).xf); }
};
/*
  get<I>(t)
*/
template <std::size_t I, tuplish T>
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
template <std::size_t I, typename T> // defined for tupl or lupl derived
auto tupl_mptr = NOT_DEFINED(tupl_mptr<I,T>);
template <std::size_t I, typename T>
  requires tupl_or_lupl<tupl_t<T>>
constexpr type_list_element_t<I,tupl_t<T>> T::* tupl_mptr<I,T>
 = [] {
  static_assert(I < tupl_size<T>, "tupl_mptr index out of bounds");
  if constexpr(I==0x00)return&T::x0;else if constexpr(I==0x01)return&T::x1;else if constexpr(I==0x02)return&T::x2;else if constexpr(I==0x03)return&T::x3;else if constexpr(I==0x04)return&T::x4;else if constexpr(I==0x05)return&T::x5;else if constexpr(I==0x06)return&T::x6;else if constexpr(I==0x07)return&T::x7;else if constexpr(I==0x08)return&T::x8;else if constexpr(I==0x09)return&T::x9;else if constexpr(I==0x0a)return&T::xa;else if constexpr(I==0x0b)return&T::xb;else if constexpr(I==0x0c)return&T::xc;else if constexpr(I==0x0d)return&T::xd;else if constexpr(I==0x0e)return&T::xe;else if constexpr(I==0x0f)return&T::xf;
  UNREACHABLE()
}();
/*
  tupl_mptrs<T> return a tupl of member pointers
*/
template <typename T, std::size_t...I>
  requires tupl_or_lupl<tupl_t<T>>
constexpr auto tupl_mptrs = tupl_mptrs<T,I...,sizeof...(I)>;
//
template <typename T, std::size_t...I>
  requires (tupl_or_lupl<tupl_t<T>> && tupl_size<T> == sizeof...(I))
constexpr auto tupl_mptrs<T,I...> = tupl{tupl_mptr<I,T>...};
/*
  assign_elements(t,v...) copy-or-move assigns all elements of a tupl
*/ // admits non-narrowing conversions
template <tuplish L, typename...T>
constexpr auto&& assign_elements(L&& l, T&&...v)
  noexcept(types_all<is_lval_nothrow_assignable,tupl_t<L>,tupl<T...>>)
  requires types_all<is_lval_assignable,tupl_t<L>,tupl<T...>>
{
  map(static_cast<tupl_like_t<L>&>(l),
      [&](auto&...a) noexcept(
           types_all<is_lval_nothrow_assignable,tupl_t<L>,tupl<T...>>)
      { ((assign(a) = (T&&)v),...); });
  return (L&&)l;
}
/*
  swap(ta,tb) tupl swap specialization, includes const-assignable,
             (std::swap already handles possibly-nested C array)
*/
template <tuplish L>
constexpr void swap(L& l, L& r)
  noexcept(types_all<std::is_nothrow_swappable, tupl_t<L>>)
  requires types_all<std::is_swappable, tupl_t<L>>
{
  enum:bool {X = types_all<std::is_nothrow_swappable, tupl_t<L>>};
  map(static_cast<tupl_like_t<L>&>(l), [&r](auto&...t) noexcept(X) {
    map(static_cast<tupl_like_t<L>&>(r), [&t...](auto&...u) noexcept(X)
    {
      (std::ranges::swap(t,u),...);
    });
  });
}
/*
  assign_to<tupl> customization for tuple-tuple assignment
*/
template <tuplish Lr>
struct assign_to<Lr>
{
  using L = tupl_t<Lr>;
  using value_type = std::conditional_t<tupl_tie<L>, const L, L>;
  value_type& l;
  template <tuplish R>
  constexpr value_type& assign_from(R&& r) const
    noexcept(types_all<is_lval_nothrow_assignable,L,tupl_t<R>>)
    requires types_all<is_lval_assignable,L,tupl_t<R>>
  {
    enum:bool{ X = types_all<is_lval_nothrow_assignable,L,tupl_t<R>> };
    if constexpr (tupl_tie<L>)
      // copy or move assign according to RHS element reference types
      [&]<template<typename...>class W
         ,typename...U>(W<U...>const& r) noexcept(X)
      {
        map(r, [&](auto&...u) noexcept(X) {
          assign_elements(l, (U)u...);
        });
      }(static_cast<tupl_t<R>const&>(r));
    else
      // copy or move assign all elements decided by RHS reference type
      map(static_cast<tupl_like_t<R&&>>(r), [&](auto&&...u) noexcept(X)
      {
          assign_elements(l, (decltype(u))(u)...);
      });
    return l;
  }
  constexpr value_type& operator=(L const& r) const
    noexcept(std::is_nothrow_copy_assignable_v<L>)
    requires (tupl_val<L> && std::is_copy_assignable_v<L>)
  {
    return l = r;
  }
  constexpr value_type& operator=(L&& r) const
    noexcept(std::is_nothrow_move_assignable_v<L>)
    requires (tupl_val<L> && std::is_move_assignable_v<L>)
  {
    return l = (L&&)r;
  }
  // assign from other tuplish
  template <tuplish R>
  constexpr value_type& operator=(R&& r) const
    noexcept(types_all<is_lval_nothrow_assignable,L,tupl_t<R>>)
    requires types_all<is_lval_assignable,L,tupl_t<R>>
  {
    return assign_from((R&&)r);
  }
  // move assign from braced init-list of rvalues; better match
  constexpr value_type& operator=(tupl_move_t<L>&& r) const
    noexcept(types_all<is_nothrow_move_assignable,L>)
    requires types_all<is_move_assignable,L>
  {
    map(r, [&](auto&...u)
     noexcept(types_all<is_nothrow_move_assignable,L>)
    {
      assign_elements(l, (tupl_move_t<decltype(u)>)(u)...);
    });
    return l;
  }
  // copy assign from braced init-list of lvalues; worse match
  template <typename...>
  constexpr value_type& operator=(tupl_view_t<L> const& r) const
    noexcept(types_all<is_nothrow_copy_assignable,L>)
    requires (types_all<is_copy_assignable,L>)
  {
    return assign_from(r);
  }
  // assign from empty braces
  constexpr value_type& operator=(std::true_type) const
    noexcept(types_all<is_nothrow_empty_list_assignable,L>)
    requires types_all<is_empty_list_assignable,L>
  {
    map(l, [](auto&...t)
     noexcept(types_all<is_nothrow_empty_list_assignable,L>)
    {((assign(t)={}),...);});
    return l;
  }
};
/*
  compare3way(l,r): Lexicographic 3-way comparison of tuplish types
                    compares any sets of 3-way comparable element types,
                    the smaller size tupl comparing less all else equal.
*/
template <int...I, tuplish L, tuplish R>
constexpr auto compare3way(L const& l,R const& r) noexcept
  requires (tupl_size<L> != tupl_size<R>
         || types_all<is_three_way_comparable_with,tupl_t<L>,tupl_t<R>>)
{
  enum { endl = tupl_size<L>
       , endr = tupl_size<R> };
  constexpr auto lr = endl<=>endr;
  constexpr auto end = lr < 0 ? endl : endr;
  constexpr compare_three_way cmp;
  if constexpr (constexpr int J{I...}; J == end) return lr;
  else if constexpr (sizeof...(I) == 0) return compare3way<0>(l,r);
  else {
    auto c = cmp(get<J>(l), get<J>(r));
    return c != 0 ? c : compare3way<J+1>(l,r);
  }
}
// compare3way(l,r) overload accepting braced init-list RHS
//
template <tuplish L>
constexpr auto compare3way(L const& l, tupl_view_t<L> r,auto...)noexcept
  { return compare3way(as_tupl_t(l), r); }
/*
  equals(l,r): equality comparison of same-arity tuplish types
               requires all equality comparable element types
*/
template <tuplish L, tuplish R>
constexpr bool equals(L const& l, R const& r) noexcept
  requires types_all<is_equality_comparable_with,tupl_t<L>,tupl_t<R>>
{
  return map(as_tupl_t(l), [&r](auto&...lh) noexcept {
    return map(as_tupl_t(r), [&lh...](auto&...rh) noexcept
    {
      return (equal_to{}(lh,rh) && ...); // no short circuit, intended;
    }); // use compare3way(l,r) == 0
  });
}
// equals(lhs,rhs) overload accepting braced init-list RHS
//
template <tuplish L>
constexpr bool equals(L const& l, tupl_view_t<L> r, auto...) noexcept
{
  return equals(as_tupl_t(l), r);
}
// equals(lhs,rhs) overload accepting empty braced init-list RHS
//
template <tuplish L>
constexpr bool equals(L const& l, std::true_type) noexcept
{
  return map(as_tupl_t(l), [](auto&...ls) noexcept
  {
    return (equal_to{}(ls,{}) && ...); // no short circuit
  });
}
//
// operator<=> overload, when possible and not defaulted in class
//
template <tuplish T>
  requires (! types_all<is_member_default_3way,tupl_t<T>>
           && types_all<is_three_way_comparable,tupl_t<T>>)
constexpr auto operator<=>(T const& l,T const& r) noexcept
{
  return compare3way(l,r);
}
//
// operator== overloads, when possible and not defaulted implicitly
// (Test that these overloads are not ambiguous)
//
template <tuplish T>
  requires (! types_all<is_member_default_3way,tupl_t<T>>
           && types_all<is_three_way_comparable,tupl_t<T>>)
constexpr auto operator==(T const& l,T const& r) noexcept
{
    return l <=> r == 0; // short circuits
}
//
template <tuplish T>
  requires (! types_all<is_member_default_3way,tupl_t<T>>
         && ! types_all<is_three_way_comparable,tupl_t<T>>)
constexpr bool operator==(T const& l,T const& r) noexcept
{
  return equals(l,r); // no short circuit
}
// Generalized integer_sequence types
// seq_T<T,v...> sequence of values of structural type T
//
template <typename T, T...v> struct seq_T {};
//template<auto f,int...I>
INT_SEQ_MAP(using type = seq_T<decltype(f(0)),f(I)...>)
// seq_map_t<f,N> = seq_T of values f(I)... for I in [0..N)
//
template <auto f, int N> using seq_map_t = SEQ_MAP();
// int_seq_t<N,B,S> = int seq of N ints starting at B with step S
//                  B + S*[0..N)
//
template <int N, int B = 0, int S = 1>
using int_seq_t = seq_map_t<[](int i){return B+S*i;},N>;
// Kronecker index sequence {i,j} : {S index, S element index}
//
template <int...siz> struct kron_t
{
  tupl<int,int> ij[(siz + ...)];
  consteval kron_t() noexcept {
    const int sizes[]{siz...};
    for (int k=0, c=0; auto& [i,j] : ij) {
      while (sizes[k] == c) {++k; c=0;}
      i = k; j = c++;
    }
  }
  constexpr auto operator()(int s) const noexcept
  {
    return ij[s];
  }
};
// kron_seq_t<s...> Kronecker index sequence of (s+...) tupl<int,int>{i,j}
//
template <int...siz>
using kron_seq_t = seq_map_t<kron_t<siz...>{},(siz+...)>;
// ctad_t<X,v> 'CTAD type'; the type that X{v{}} element 0 would have
//             if X is a tuplish template id with value category CTAD
//             and as if deduced array values initialize ok.
//
template <template<typename...>class X, typename v>
using ctad_t = copy_cvref_t<type_list_element_t<0,
     decltype(X{std::declval<all_extents_removed_t<v>>()})>, v>;
namespace impl { // helpers to compute concatenated tupl types
template <tuplish...TL, auto...IJ>
constexpr auto cat_t(seq_T<tupl<int,int>, IJ...>)
  -> tupl<type_list_element_t<IJ.x1,
          type_list_element_t<IJ.x0, tupl<TL...>>>...>;
template <template<typename...>class X, tuplish...TL, auto...IJ>
constexpr auto cat_ctad_t(seq_T<tupl<int,int>, IJ...>)
  -> X<ctad_t<X,type_list_element_t<IJ.x1,
                type_list_element_t<IJ.x0, tupl<TL...>>>>...>;
} // impl
// cat_t<X,TL...> -> X<E...> concatenation type of tuplish element types
//
template <tuplish...TL>
using cat_t = decltype(impl::cat_t<std::remove_cvref_t<TL>...>
                                  (kron_seq_t<tupl_size<TL>...>{}));
// cat_ctad_t<X,TL...> -> decltype( X{fwd<E>()...} )
// concatenation type of forwarded element types as deduced by X's CTAD
//
template <template<typename...>class X, tuplish...TL>
using cat_ctad_t = decltype(impl::cat_ctad_t<X,tupl_fwd_t<TL>...>
                                  (kron_seq_t<tupl_size<TL>...>{}));
template <typename V, typename T>
concept constructs = is_constructible_v<T,V>;
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
constexpr auto tupl_init(constructs<T> auto&&...v)
            noexcept((is_nothrow_constructible_v<T,decltype(v)> && ...))
  -> X<T...>
            requires (sizeof...(T) == sizeof...(v))
{
  if constexpr (requires {X<T...>{(decltype(v))v...};})
    return {(decltype(v))v...}; // no arrays present, aggregate init
  else // expand array elements in brace-elided aggregate initializers
    return [&]<auto...IJ>(seq_T<tupl<int,int>, IJ...>)
            noexcept((is_nothrow_constructible_v<T,decltype(v)> && ...))
          -> X<T...>
    {
      NO_WARN_MISSING_BRACES(
      return {flat_index(get<IJ.x0>(fwds{(decltype(v))v...}),IJ.x1)...};
      )
    }(kron_seq_t<flat_size<std::remove_cvref_t<T>>...>{});
}
/*
  tupl_init(v...) -> tupl{v...};  with tupl CTAD
  tupl_init<X>(v...) -> X{v...};  with template id X CTAD
*/
template <template<typename...>class X = tupl, typename...T>
constexpr auto tupl_init(auto&&...v) noexcept(
     noexcept(tupl_init<X,ctad_t<X,decltype(v)>...>((decltype(v))v...)))
  -> X<ctad_t<X,decltype(v)>...>
    requires (sizeof...(T) == 0 && sizeof...(v) != 0 &&
    requires{tupl_init<X,ctad_t<X,decltype(v)>...>((decltype(v))v...);})
{
  return tupl_init<X,ctad_t<X,decltype(v)>...>((decltype(v))v...);
}
// cat(t...) concatenate tuplish t's preserving element types (no CTAD)
//
template <tuplish...TL>
constexpr auto cat(TL&&...tl) noexcept(
 (std::is_nothrow_constructible_v<std::remove_cvref_t<TL>,TL&&> && ...))
 -> cat_t<tupl_t<TL>...>
 requires(std::is_constructible_v<std::remove_cvref_t<TL>,TL&&> && ...)
{
  return [&]<auto...IJ>(seq_T<tupl<int,int>,IJ...>) noexcept(
 (std::is_nothrow_constructible_v<std::remove_cvref_t<TL>,TL&&> && ...))
 -> cat_t<tupl_t<TL>...>
  {
    return tupl_init<tupl,type_list_element_t<IJ.x1,
                          type_list_element_t<IJ.x0, tupl<TL...>>>...>
                         (get<IJ.x1>(get<IJ.x0>(fwds{(TL&&)tl...}))...);
  }
  (kron_seq_t<tupl_size<TL>...>{});
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
  return [&]<auto...IJ>(seq_T<tupl<int,int>,IJ...>) noexcept(
     (std::is_nothrow_constructible_v<cat_ctad_t<X,TL&&>,
                   apply_cvref_t<TL&&,cat_ctad_t<X,TL&&>>> && ...))
    -> cat_ctad_t<X,TL&&...>
  {
    return tupl_init<X>(get<IJ.x1>(get<IJ.x0>(fwds{(TL&&)tl...}))...);
  }
  (kron_seq_t<tupl_size<TL>...>{});
}
