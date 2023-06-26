#include <tupl/tupl.hpp>
#include <cassert>

void ff() {
constexpr auto mps = lml::tupl_mptrs<lml::tupl<int,bool>>;
//auto const& [ti,tb] = lml::tupl_mptrs<lml::tupl<int,bool>>;
constexpr auto ti = mps.x0;
constexpr auto tb = mps.x1;

constexpr lml::tupl tib = {1,true};

static_assert( tib.*ti == 1 && tib.*tb );
static_assert( ti == &lml::tupl<int,bool>::x0 );
}

template <lml::c_array<char> A>
consteval bool ntbs(A const& a) {
  for (auto& c : a)
    if (c == 0)
      return &c == &a[sizeof(a)-1];
}

template <lml::tupl s>
  requires (lml::tupl_size_v<decltype(s)> == 1
            && lml::c_array<decltype(s.x0),char>
            && ntbs(s.x0))
struct tstring : decltype(s) {
  consteval tstring() : decltype(s)(s) {}
};

//using dmr = tstring<{"DMR"}>;

//static_assert( dmr{} == lml::tupl{"DMR"} );

lml::tupl t0;
static_assert( std::same_as<decltype(t0),lml::tupl<>> );

constexpr lml::tupl t012 = {0,1U,"2"};

auto tupl_API(lml::tupl<int,unsigned,char[2]> t = t012)
{
  auto s{t}; // Copy construct, as struct aggregate
  assert(s == t); // Only same types are comparable

  s = t; // Copy-assign, aggregate struct builtin
  swap(s,t); // Specialized elementwise lml::swap

  t = {}; // Clear by builtin 'aggregate assignment'
  assert(equals(t,{})); // t == {} syntax is illegal

  t = {1,2,"3"}; // builtin 'aggregate assignment'

  auto& [i,u,c2] = t; // Structured binding access
                      // by builtin aggregate rule

  t = {i,u}; // WARN of missing initializer, and...
//t = {u,i}; // WARN or FAIL, narrowing conversions

//t = {i,u,c2}; // FAIL: array variable initializer

  assign(t) = {i,u,c2}; // Handles array variables,
/*                        (all initializers needed)
  Note:
  assign from braced list can only copy-assign from
  or move-assign from all initializers, not a mix.

  assign_elements allows elementwise move or copy:
*/
  assign_elements(t,u,i,c2);

  t.x0 = {1}; // direct access by known member id
  get<1>(t) = {2}; // usual indexed get<I> access
//get<2>(t) = {"3"}; // FAIL: can't assign arrays

  lml::assign(get<2>(t)) = {"3"}; // array assign
/*
  getie<2>(t) = {"3"}; // From <tupl/tupl_tie.hpp>
*/
  using lml::flat_index; //(1st array elem& or arg&)

  // This map adds 3 to each element of tupl t:
  map(t ,[](auto&...a){((flat_index(a)+=3),...);});

  assert((t == lml::tupl{4,5U,"6"}));

  return lml::tupl{t, true}; // nested tupl return
}

//static_assert( sizeof(tupl_API()) == 16 ); // GCC = 12

#include <tupl/tupl_tie.hpp>  // ties operator= overloads
#include <tupl/tupl_cmps.hpp> // cmps operator== & <=>

bool tie_API(int i, unsigned u, char(&c2)[2])
{
  bool z = equals(lml::tie(i,u,c2), {});

  z = z && lml::tie(i,u,c2) == lml::cmps{0,0U,"\0"};
  z = z && lml::tie(i,u,c2) == lml::cmps{{}};

  lml::ties refs = {i,u,c2}; // non-const qualified
// refs = {i,u,c2}; // error: ambiguous overload
  using as_const = decltype(refs) const&;
  (as_const)refs = {i,u,c2};

  lml::tie(i,u,c2) = {}; // clear all to ={} init
  lml::tie(i,u,c2) = {1,2,"3"}; // assign-through
  lml::tie(c2) = {{'4'}};  // assign from array rvalue

  getie<2>(refs) = {"5"};   // assign to array element
  getie<0,1>(refs) = {3,4}; // multi-index get -> tie

  return z;
}

#include <cassert>

using namespace lml;

#if defined (_MSC_VER)
#define MSVC(...)__VA_ARGS__
#else
#define MSVC(...)
#endif


static_assert( tupl{1,2L,'3'} > tupl{1,2L,'2'} );

int mm;
same_ish<int> auto&& nn = mm;


struct c16 { char data[16]; };
struct BIG { char data[20]; };

static_assert( std::same_as<tupl_view_t<tupl<int&,long&,char&>>,
                                        tupl<int, long, char>> );

static_assert( std::same_as<tupl_view_t<tupl<int[20]>>,
                                        tupl<int const (&)[20]>> );

static_assert( std::same_as<tupl_view_t<tupl<int(&)[20]>>,
                                        tupl<int const(&)[20]>> );
static_assert( std::is_trivially_copyable<tupl<int(&)[20]>>{} );

static_assert( std::same_as<tupl_view_t<tupl<char[16]>>,
                                        tupl<char const(&)[16]>> );

static_assert( std::same_as<tupl_view_t<tupl<BIG>>,
                                        tupl<BIG const&>> );


void big() {
    tupl c {c16{}};
    c = {};
    c16 cc{};
    c = {cc};

    tupl b {BIG{}};
    b = {};
    BIG bb{};
    b = {bb}; // default

    //tupl<BIG&> br{bb};
    //br = {};
    //br = {{}};
    //tupl<BIG&> const cbr{bb};
    //cbr = {};
    //cbr = {{}};
    //BIG bc{};
    //br = {bc}; // ambiguous tupl<BIG&> deleted, tupl<BIG&
    //cbr = {bc};  // not ambiguous
    //cbr = {"loo"};
    //br = {(BIG const&)bc}; // disambiguated - exact match
//    assert( )
}

tupl tup = {"c++",true}; // deduces tupl<char[4],bool>

using Tup = decltype(tup);

static_assert( std::is_same_v< Tup, tupl<char[4],bool>>
            && std::is_aggregate_v< Tup >
            && std::is_standard_layout_v< Tup >
            && std::is_trivial_v< Tup >
            && std::is_trivially_copyable_v< Tup >
);
template<tupl> struct structural;
using is_structural = structural<tupl{"tupl"}>;

template <auto> using isstructural = std::true_type;
static_assert(        isstructural< Tup{} >() );

using tupl_scalar_refs = ties<int&, long&, char&>;

static_assert( std::is_aggregate<tupl_scalar_refs>() );
static_assert( std::is_trivially_copyable<tupl_scalar_refs>{});
static_assert(!std::is_trivially_default_constructible<tupl_scalar_refs>());
static_assert( MSVC(!) std::is_trivial_v<tupl_scalar_refs>);

#include <memory>
#include <tupl/tupl_vals.hpp>

auto tup_copy_assign(char(&cstr)[4],bool b)
{
  vals<char[4],bool> rupl = {"c++",true};
  rupl = {std::move(cstr),std::move(b)};
//  assign_to{tup} = {cstr,b}; // error: no match for ‘operator=’
  assign_elements(tup, cstr,b);
  assign(tup) = tie(cstr,b);
  return tup = (vals<char[4],bool>{} = {cstr,b});
}

#include <iostream>
using std::ostream;

ostream& operator<<(ostream& out, tuplish auto const& t)
{
  return map(as_tupl_t(t), [&out](auto&...a) -> auto&
  {
    char sep = '{';
    auto sep_out = [&]{
           out << sep;
           [[maybe_unused]]static auto _(sep = ',');
         };

    return (out << ... << (sep_out(), a)) << '}';
  });
}

#include <tupl/tupl_cat.hpp>

void catr()
{
  auto fwds_cat = []<lml::tuplish...T>(T&&...t){
    return lml::cat<lml::fwds>((T&&)t...);
  };
  lml::fwds ff = fwds_cat(tupl{"a"},tupl{"b","c"});
  static_assert(std::same_as<decltype(ff),lml::fwds<char(&&)[2],char(&&)[2],char(&&)[2]>>);
}

#include <string>
using namespace std::string_literals;
//static_assert( "c++"s == "c++"); // not constexpr on clang yet

tupl stringstd{"c++"s,20};

int main()
{
  tupl<std::string> strng = {"v"};
  strng = {"X"};
  assert((stringstd == cmps{+"c++",20}));

  tupl<char[4],int> cppstd; // uninitialized
  cppstd = {"c++",20};     // 'aggregate assignment'
  cppstd = {};
  cppstd = tupl<char[4],int>{"c++",20}; // same as above

  char cpp[]{"c--"};  // array lvalue variable
  int std = 20;
//cppstd = {cpp, }; // FAIL: array lvalue won't init

  assign(cppstd) = {cpp, 20}; // works

  auto cppxx = assign(tupl<char[4],int>{}) = {cpp,std};
  auto cppyy = lml::tupl_init(cpp,std);
  assert( cppxx == cppyy );
  cppxx = cppyy;

  vals<char[4],int> cppval; // uninitialized
  cppval = {cpp, std};      // two-phase init
  cppval = lml::tie_fwd(cpp,'\x14');

  // two-phase init one-liner
  auto cppass = vals<char[4],int>{} = {cpp,std};
  assert( cppass == cppxx );
  cppass = cppxx;

//  assign(cstd) = {{},20};
//  assign(cstd) = {cmm,20};

  puts("testing");
  std::cout << std::boolalpha << tupl_API() << "\n";
  //ass();
  //test_swap();
  //test_refs();

  //lml::tupl mut {1,2L,'3'};
  //lml::tupl mvt {1,2L,'2'};
  //mut = {};
  //mut = {42}; // missing initializer warning
  //mut = {2,3,4}; // missing initializer warning
  //    return mut <=> mut == 0
  //        && mut <=> mvt > 0
  //        && compare(mut, {1,2,2}) == 0;
  //return !(mut <=> mut == 0
  //    && mut <=> mvt < 0
  //&& compare3way(mut, mvt) < 0);
}

//constexpr tupl mu {1,2L,'3'};
//static_assert( compare(mu, {1,2,2}) > 0 );
