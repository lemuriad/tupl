#include "tupl.hpp"

int main()
{
  static_assert(lml::tuplish<lml::tupl<int>&>);
  return get<0>(lml::tupl{0});
}
