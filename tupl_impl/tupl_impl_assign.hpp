/*
  assign_elements(t,v...) copy-or-move assigns all elements of a tupl
*/                                  // admits non-narrowing conversions
template <tuplish L, typename...T>
constexpr auto& assign_elements(L& l, T&&...v)
  noexcept(types_all<is_nothrow_assignable, tupl_fwd_t<L&>, tupl<T...>>)
  requires types_all<is_assignable, tupl_fwd_t<L&>, tupl<T...>>
{
  map(as_tupl_t(l),
      [&](auto&...a) noexcept(
           types_all<is_nothrow_assignable, tupl_fwd_t<L&>, tupl<T...>>)
      { ((assign(a) = (T&&)v),...); });
  return (L&)l;
}

/*
  swap(ta,tb) tupl swap specialization, includes const-assignable,
             (std::swap already handles possibly-nested C array)
*/
template <tuplish L, same_ish<std::remove_cvref_t<L>> R>
constexpr void swap(L&& l, R&& r)
  noexcept(types_all<std::is_nothrow_swappable, tupl_t<L>>)
  requires types_all<std::is_swappable, tupl_t<L>>
{
  enum:bool {X = types_all<std::is_nothrow_swappable, tupl_t<L>>};
  map(as_tupl_t(l), [&r](auto&...t) noexcept(X) {
    map(as_tupl_t(r), [&t...](auto&...u) noexcept(X)
    {
      (std::ranges::swap(t,u),...);
    });
  });
}
