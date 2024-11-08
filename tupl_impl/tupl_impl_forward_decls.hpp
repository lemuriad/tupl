/*
   tupl<E...> primary template declaration; an aggregate struct
   with [[no_unique_address]] attribute on all element member decls E...
*/
template <typename...E> struct tupl;  // aggregate [[no_unique_address]]
/*
   tupl CTAD deduces elements by-value, including arrays (and functions)
*/
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
//
template <typename...E> struct vals;
template <typename...E> vals(E const&...) -> vals<E...>;
//
template <typename...E> struct ties;
template <typename...E> ties(E&...) -> ties<E&...>;
//
template <typename...E> struct cmps;
template <typename...E> cmps(E const&...) -> cmps<tupl_view_t<E>...>;
/*
   Derived tuplish types with custom CTAD only, nothing else added
*/
template <typename...E> struct fwds  : tupl<E...> {}; // forwarding refs
template <typename...E> struct lvals : tupl<E...> {}; // lvalue refs
template <typename...E> struct rvals : tupl<E...> {}; // rvalue refs
template <typename...E> struct cvals : tupl<E...> {}; // const view vals
/*
   CTAD guides
*/
template <typename...E> fwds(E&&...) -> fwds<E&&...>;
template <typename...E> lvals(E&...) -> lvals<E&...>;
template <rvalue... RV> rvals(RV&&...) -> rvals<RV&&...>;
template <typename...E> cvals(E const&...) -> cvals<tupl_view_t<E>...>;
//