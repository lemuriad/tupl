#include "tupl_impl_int_seq.hpp"

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
  -> X<ctad_t<X,decltype(v)>...> requires (sizeof...(T) == 0 &&
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
