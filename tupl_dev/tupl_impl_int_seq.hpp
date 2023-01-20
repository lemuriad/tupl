// Generalized integer_sequence types

// seq_T<T,v...> sequence of values of structural type T
//
template <typename T, T...v> struct seq_T {};

//template<auto f,int...I>
INT_SEQ_MAP(using type = seq_T<decltype(f(0)),f(I)...>)

// seq_map_t<f,N> = seq_T of values f(I)... for I in [0..N)
//
template <auto f, int N> using seq_map_t = SEQ_MAP();

// int_seq_t<N,B,S> = int seq of N ints starting at B with step S
//                  B + S*[0..N)
//
template <int N, int B = 0, int S = 1>
using int_seq_t = seq_map_t<[](int i){return B+S*i;},N>;

// Kronecker index sequence {i,j} : {S index, S element index}
//
template <int...siz> struct kron_t
{
  struct {int i,j;} ij[(siz + ...)];

  consteval kron_t() noexcept {
    const int sizes[]{siz...};
    for (int k=0, c=0; auto& [i,j] : ij) {
      while (sizes[k] == c) {++k; c=0;}
      i = k; j = c++;
    }
  }
  constexpr auto operator()(int s) const noexcept
  {
    return ij[s];
  }
};
// kron_seq_t<s...> Kronecker index sequence of (s+...) tupl<int,int>{i,j}
//
template <int...siz>
using kron_seq_t = seq_map_t<kron_t<siz...>{},(siz+...)>;
