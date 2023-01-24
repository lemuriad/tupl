#include "tupl_amalgam.hpp"

constexpr lml::tupl t{0};

static_assert(t == lml::cmps{0});

int main()
{
  int a4[4]{1,2,3,4};

  lml::vals<int[4],bool> ab{};
  ab = {a4,true};

  lml::tie(a4) = {{2,3,4,5}};
}
