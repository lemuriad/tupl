/*
 SPDX-FileCopyrightText: 2023 The Lemuriad <opensource@lemurianlabs.com>
 SPDX-License-Identifier: BSL-1.0
 Repo: https://github.com/Lemuriad/tupl
*/

// Generalized integer_sequence types

// INT_SEQ_MAP & SEQ_MAP platform-specific macros
// wrap integer-sequence generator builtins
//
#if defined(__clang__)

#define INT_SEQ_MAP(...)\
template<auto f>struct int_seq_map{template<class,int...I>__VA_ARGS__;};
#define SEQ_MAP()__make_integer_seq<int_seq_map<f>::template type,int,N>

#elif defined(__GNUG__)

#define INT_SEQ_MAP(...)\
namespace int_seq_map{template<auto f,int...I>__VA_ARGS__;}
#define SEQ_MAP()int_seq_map::type<f,__integer_pack(N)...>

#elif defined(_MSC_VER)

#define INT_SEQ_MAP(...)template<auto f>struct int_seq_map{\
template<class,int...I>struct ty{__VA_ARGS__;};};
#define SEQ_MAP()\
typename __make_integer_seq<int_seq_map<f>::template ty,int,N>::type;

#endif

#include "namespace.hpp"

// ij_t is a pair of int indices i,j
//
struct ij_t {int i,j;};

// val_seq<T,v...> sequence of values of structural type T
//
template <typename T, T...v> struct val_seq {};

//template<auto f,int...I>
INT_SEQ_MAP(using type = val_seq<decltype(f(0)),f(I)...>)

// val_seq_map<f,N> = val_seq of values f(I)... for I in [0..N)
//
template <auto f, int N> using val_seq_map = SEQ_MAP();

// int_seq<N,B,S> = int seq of N ints starting at B with step S
//                  B + S*[0..N)
//
template <int N, int B = 0, int S = 1>
using int_seq = val_seq_map<[](int i){return B+S*i;},N>;

namespace impl {
// ij_map<s...> is a 2d index sequence of int pairs {i,j}
// where i indexes the s... and j indexes the jth element [0..s)
//
template <int...siz> struct ij_map
{
  ij_t ij[(siz + ...)];

  consteval ij_map() noexcept {
    const int sizes[]{siz...};
    for (int k=0, c=0; auto& [i,j] : ij) {
      while (sizes[k] == c) {++k; c=0;}
      i = k; j = c++;
    }
  }
  consteval auto operator()(int s) const noexcept
  {
    return ij[s];
  }
};
template <int...siz> requires ((siz+...+0)==0) struct ij_map<siz...>
{
  consteval ij_t operator()(int) const noexcept {return{};}
};
} // impl

// ij_seq<s...> 2D index sequence of (s+...) ij_t int pairs
//
template <int...siz>
using ij_seq = val_seq_map<impl::ij_map<siz...>{},(siz+...+0)>;

#undef SEQ_MAP
#undef INT_SEQ_MAP

#include "namespace.hpp"
