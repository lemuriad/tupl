/*
  assign_elements(t,v...) copy-or-move assigns all elements of a tupl
*/                                  // admits non-narrowing conversions
template <tuplish L, typename...T>
constexpr auto&& assign_elements(L&& l, T&&...v)
  noexcept(types_all<is_lval_nothrow_assignable,tupl_t<L>,tupl<T...>>)
  requires types_all<is_lval_assignable,tupl_t<L>,tupl<T...>>
{
  map(as_tupl_t(l),
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
  map(as_tupl_t(l), [&r](auto&...t) noexcept(X) {
    map(as_tupl_t(r), [&t...](auto&...u) noexcept(X)
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
      map(as_tupl_t((R&&)r), [&](auto&&...u) noexcept(X)
      {
          assign_elements(l, (decltype(u))(u)...);
      });
    return l;
  }

  constexpr value_type& operator=(L& r) const
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
