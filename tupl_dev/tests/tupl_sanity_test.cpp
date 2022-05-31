#include "tupl.hpp"

int main()
{
  static_assert(tuplish<tupl<int>&>);
  return get<0>(tupl{0});
}
