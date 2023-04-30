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

#ifndef TUPL_COMPARE3WAY_DIRECT // default tail-recursive implementation

  if constexpr (constexpr int J{I...}; J == end) return lr;
  else if constexpr (sizeof...(I) == 0) return compare3way<0>(l,r);
  else {
    auto c = cmp(get<J>(l), get<J>(r));
    return c != 0 ? c : compare3way<J+1>(l,r);
  }

#else // alternative non-recursive implementation unrolls direct access
#define MACRO(N)\
if constexpr(HEXLIT(N)>=end)return lr;\
else {if(auto c=cmp(l.TUPL_DATA_ID(N),r.TUPL_DATA_ID(N));c!=0)return c;}

  if constexpr (end == 0) return lr;
  XREPEAT(TUPL_MAX_ARITY,MACRO,NOSEP)
  UNREACHABLE()

#undef MACRO
#endif
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
    });                                  // use compare3way(l,r) == 0
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
