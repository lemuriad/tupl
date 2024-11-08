/*
  get<I>(t)
*/
template <size_t I, tuplish T>
constexpr auto get(T&& t) noexcept
-> apply_cvref_t<T&&,type_list_element_t<I,tupl_t<T>>>
  requires (I < tupl_size_v<T>)
{
  using tuplt = tupl_t<T>;
  using ret_t = apply_cvref_t<T&&,type_list_element_t<I,tuplt>>;
  using ptr_t = std::remove_reference_t<ret_t>*;

  struct E{constexpr E(...){}};
  struct At{ E a[I+1]; ptr_t e; E b[tupl_size_v<tuplt>-I]; };

  return map(static_cast<tupl_like_t<T>&>(t), [](auto&...a) noexcept
    -> ret_t
  {
    NO_WARN_MISSING_BRACES()
      return static_cast<ret_t>(*At{0,&a...,0}.e);
    END_NO_WARN_MISSING_BRACES()
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
constexpr auto tupl_mptr<I,T>
 = [] {
  static_assert(I < tupl_size_v<T>, "tupl_mptr index out of bounds");
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
  requires (tupl_or_lupl<tupl_t<T>> && tupl_size_v<T> == sizeof...(I))
constexpr auto tupl_mptrs<T,I...> = tupl{tupl_mptr<I,T>...};
