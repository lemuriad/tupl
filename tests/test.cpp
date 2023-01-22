#include "tupl_traits.hpp"

#define SAME(...)static_assert(std::is_same_v<__VA_ARGS__>)

#define REMOVE_REF_T(...) std::remove_reference_t<__VA_ARGS__>
#if __cpp_lib_remove_cvref
#define REMOVE_CVREF_T(...) std::remove_cvref_t<__VA_ARGS__>
#else
#define REMOVE_CVREF_T(...) std::remove_cv_t<REMOVE_REF_T(__VA_ARGS__)>
#endif

// completeness test
template <typename T, typename = void>
  std::false_type complete;
template <typename T>
  std::true_type complete<T, std::void_t<decltype(sizeof(T))>>;

// Traits tests only

// tuplish

static_assert( ! tuplish<int[1024]> );

// The tuplish concept is minimal and lax, for now -
//  a tupl_t member alias, convertible to the super type
//  a map(t,f) function
//
template <typename...T> struct nomaptupl { using tupl_t = nomaptupl; };
static_assert( ! tuplish<nomaptupl<>> );

template <typename...T> struct tuplfriend {
  using tupl_t = tuplfriend;
  friend auto map(tuplfriend, auto f){return f();}
};
static_assert( tuplish<tuplfriend<>> );
static_assert( tuplish<tuplfriend<void>> );

template <typename...T> struct tuplfree {
  using tupl_t = tuplfree;
};
template <typename...T>
auto map(tuplfree<T...> const&, auto f){return f();}
static_assert( tuplish<tuplfree<>> );
static_assert( tuplish<tuplfree<void>> );

// non-template derived classes are still tuplish
//
struct supertuplfriend : tuplfriend<int> {};
struct supertuplfree : tuplfree<int> {};

static_assert( tuplish<supertuplfriend> );
static_assert( tuplish<supertuplfree> );

static_assert( tupl_size<supertuplfree> == 1 );

// type_list test

template <typename...> struct type_list;
template <typename...> struct type_pack;

static_assert( type_list_size< type_list<char,short,int,long> > == 4 );

static_assert( types_all<std::is_integral, type_list<char,short,int,long> > );
static_assert( ! types_all<std::is_integral, type_list<char,short,int,void> > );

template <typename L, typename R> using bigger = std::bool_constant<
           (sizeof(L) > sizeof(R))>;

static_assert( types_all<bigger, type_list<short,float,double>
                                , type_list<char,char[3],float> > );

// type_list_element test

SAME( type_list_element_t<2,type_list<int,bool,void,long>>, void );
static_assert( ! type_pack_element_t<1,int,bool,void,long>{} );

SAME( type_list_element_t<16,type_list<
int[ ],int[1],int[2],int[3],int[4],int[5],int[6],int[7],
int[8],int[9],int[10],int[11],int[12],int[13],int[14],int[15],void>>, void );

static_assert( std::extent_v<type_list_element_t<15, type_list<
int[ ],int[1],int[2],int[3],int[4],int[5],int[6],int[7],
int[8],int[9],int[10],int[11],int[12],int[13],int[14],int[15]>>> == 15);

static_assert( std::is_void_v<type_list_element_t<15, type_list<
int[ ],int[1],int[2],int[3],int[4],int[5],int[6],int[7],
int[8],int[9],int[10],int[11],int[12],int[13],int[14],void,int[16]>>> );

#include "snitch/snitch.hpp"

#include "tupl.hpp"

auto ir = fwds{1,1};
SAME(decltype(ir), fwds<int&&,int&&>);

auto ti = fwds{1,1};
SAME(decltype(ti), fwds<int&&,int&&>);

constexpr tupl tupl8 = cat(tupl{1},tupl{2},tupl{3},tupl{4}
                          ,tupl{5},tupl{6},tupl{7},tupl{8},tupl{9});
static_assert( tupl8 == tupl{1,2,3,4,5,6,7,8,9});
SAME(decltype(tupl8), const tupl<int,int,int,int,int,int,int,int,int>);

auto tupl8r = cat<fwds>(tupl8);

template<class...>class wotype;
//wotype<decltype(tupl8r)> eh;
SAME(decltype(tupl8r), fwds<int const&,int const&,int const&,int const&,
int const&,int const&,int const&,int const&,int const&>);

static_assert( get<0>(tupl{1}) == 1 );

#include <memory>

// test("tupl void")

using tupl_void = tupl<void>; // you can name the type
static_assert( type_list_size<tupl_void> == 1 ); // and use it
// but can't instantiate it (needs a compile-fail test)

// test tupl_max_arity

// make a tupl of ints from an index sequence
auto make_tupl = []<size_t...I>(std::index_sequence<I...>)
                                   -> tupl<decltype(I)...> {return {};};

using max_arity = std::make_index_sequence<tupl_max_arity>;
using max_aritypp = std::make_index_sequence<tupl_max_arity+1>;

auto max_arity_tupl = make_tupl(max_arity{});
using max_aritypp_tupl = decltype(make_tupl(max_aritypp{}));

// incomplete tupl type can be used like any generic type list
static_assert( type_list_size<max_aritypp_tupl> == tupl_max_arity+1);

// but you can't instantiate more than the max arity
static_assert( complete<decltype(max_arity_tupl)> );
static_assert( ! complete<max_aritypp_tupl> );

// tupl type categories enumeration
//  * non => Not a tupl
//  * val => Value tupl; all T is_object<T>
//  * tie => lvalue tupl; all T = U& and is_object<U>
//  * ish => Any other tupl that is not val or tie
enum class category {non, val, tie, ish};

// test("tupl categories")
void tupl_categories()
{
  auto categorize = []<typename T>(category cat)
  {
    return( tuplish<T>  == (cat!=category::non)
         && tupl_val<T> == (cat==category::val)
         && tupl_tie<T> == (cat==category::tie) );
  };
# define CATEGORY(C,...) categorize.template operator()<__VA_ARGS__>(category::C)

  static_assert( CATEGORY(non, void) );
  static_assert( CATEGORY(non, int) );
  static_assert( CATEGORY(non, std::unique_ptr<int> ) );
  static_assert( CATEGORY(non, tupl<> tupl<>::*) );

  static_assert( CATEGORY(val, tupl<int> const) );
  static_assert( CATEGORY(val, tupl<int> const volatile&) );
  static_assert( CATEGORY(val, tupl<int>&&) );
  static_assert( CATEGORY(val, tupl<int[1]>) );
  static_assert( CATEGORY(val, tupl<int(*)[1]>) );
  static_assert( CATEGORY(val, tupl<int(*)()>) );

  static_assert( CATEGORY(tie, tupl<int(&)[1]>) );
  static_assert( CATEGORY(tie, tupl<int&> const) );
  static_assert( CATEGORY(tie, tupl<int&> const volatile&) );
  static_assert( CATEGORY(tie, tupl<int&>&&) );
  static_assert( CATEGORY(tie, tupl<int&&>) );

 // static_assert( CATEGORY(tie, tupl<tupl<int&>>) );

  static_assert( CATEGORY(ish, tupl<int const&>) );
  static_assert( CATEGORY(ish, tupl<int(&)()>) );
  static_assert( CATEGORY(ish, tupl<int,int&>) );

#undef CATEGORY
}

struct derived_tupl : tupl<int> {};
static_assert( tuplish<derived_tupl> );

// Not a template - it's a tupl by inheritance
struct tuplintchar : tupl<int,char> {};
tupl t0{};

void tupl_sizes(tupl<> const& e0 = {})
{
  SAME( decltype(tupl_size<tupl<>>), const size_t );
  static_assert( tupl_size<tupl<>> == 0);
  static_assert( tupl_size<decltype(e0)> == 0);
  static_assert( tupl_size<decltype(t0)const&&> == 0);
  static_assert( tupl_size<decltype(tupl{})> == 0 );

  tupl s1{1};
  tupl s2{1,2};
  tupl sx{1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};

  static_assert( tupl_size<decltype(sx)> == 16 );
  static_assert( tupl_size<decltype(s1)> + tupl_size<decltype(s2)> == 3 );

  static_assert( tupl_size<tupl<char[1024], char>> == 2 );

  static_assert( tupl_size<tuplintchar> == 2 );
};

SAME( tupl_move_t<void>, void );

SAME( tupl_move_t<int>, int );

SAME( tupl_move_t<tupl<int>>, tupl<int> );

SAME( tupl_move_t<int[1]>, int(&&)[1] );

SAME( tupl_move_t<tupl<int[1]>>
                 ,tupl<int(&&)[1]> ); // tupl<int[1]> trivially assignable

SAME( tupl_move_t<tupl<int,void(*)(),int[1],std::unique_ptr<int[]>>>
                 ,tupl<int,void(*)(),int(&&)[1],std::unique_ptr<int[]>&&> );

SAME( tupl_move_t<int()>, int(&)() );

SAME( tupl_move_t<tupl<int(&)()>>
                 ,tupl<int(&)()> );

SAME( tupl_move_t<tupl<tupl<std::unique_ptr<int[]>>>>
                 ,tupl<tupl<std::unique_ptr<int[]>&&>> );

struct explct { explicit explct() = default; };


#include <cassert>

#include "tupl_tie.hpp"

//static_assert( tuplish<ties<int&, long int&>> );

tupl maptest = {1,'2'};

//constexpr auto maptr = [](void(&f)(int,char))->auto&{return f;};

//constexpr auto& mapic = maptr(map);

// test("tupl CTAD") // Constructor Template Argument Deduction tests

SAME( decltype(tupl{}), tupl<>);
SAME( decltype(tupl{0}), tupl<int> );
SAME( decltype(tupl{""}), tupl<char[1]> );
SAME( decltype(tupl{&tie<int&>})
             , tupl<const ties<int&>(*)(int&)noexcept>);
             // function pointer
SAME( decltype(tupl{std::make_unique<int>()})
             , tupl<std::unique_ptr<int>>);
             // move-only type test
SAME( decltype(tupl{tupl{}}) // Nested single tupl element
             , tupl<> );   // collapses (!)
SAME( decltype(tupl{tupl{0}}) // Nested single tupl element
             , tupl<int> ); // collapses !
SAME( decltype(tupl{tupl{},tupl{}}) // Nested multiple tupl elements
             , tupl<tupl<>,tupl<>> ); // no collapse

// test("tupl val assign")
void tupl_val_assign()
{
  tupl{} = tupl{};
  tupl{} = {};

  tupl<> t;
  t = {};

  static_assert( get<0>(tupl{1} = {}) == 0 );
  static_assert( get<0>(tupl{2} = {1}) == 1 );

  tupl<int> ti;
  (ti = {}) = {1};
  ti = tupl{2};

  tupl<int[2]> ti2;
  ti2 = {};
  ti2 = {0};
  ti2 = {0,1};
  ti2 = {{1,2}};
  ti2 = {.x0{1,2}};
}

// test("tuplish")

// Values for logging SMF special member function calls
constexpr auto destruct = 0; // destructor call
constexpr auto def_cons = 1; // default constructor call
constexpr auto copy_con = 2; // copy constructor call
constexpr auto move_con = 3; // move constructor call
constexpr auto copy_ass = 4; // copy assignment call
constexpr auto move_ass = 5; // move assignment call

// SMF special member function recorder type, totally non-trivial
template <bool nox, int(&op)[16]>
struct SMF
{
  constexpr ~SMF() noexcept(nox) {push(destruct);}
  constexpr SMF() noexcept(nox) {push(def_cons);}
  constexpr SMF(SMF const&) noexcept(nox) {push(copy_con);}
  constexpr SMF(SMF&&) noexcept(nox) {push(move_con);}
  constexpr SMF& operator=(SMF const&) noexcept(nox) {push(copy_ass);return*this;}
  constexpr SMF& operator=(SMF&&) noexcept(nox) {push(move_ass);return*this;}

  constexpr void push([[maybe_unused]]int smn) noexcept
  {
    op[++op[0]] = smn;
  }
};

int i16[16];
static_assert( ! std::is_aggregate_v<SMF<true,i16>> );
static_assert( ! std::is_trivial_v<SMF<true,i16>> );

// test("tupl assign SMF ops")
auto tupl_assign_SMFops()
{
  static int ops[16]{};
  auto clear = [](auto&o){for(auto&e:o)e=0;};

  using smn = SMF<true,ops>;  // noexcept
  using smx = SMF<false,ops>; // throwing

  (void)tupl{smn{}};
  assert(ops[0] == 2);
  assert(ops[1] == def_cons);
  assert(ops[2] == destruct);

  clear(ops);
  tupl<smn> tnox{};
  assert(ops[0] == 1);
  assert(ops[1] == def_cons);

// noexcept
  auto noxt = tnox;    // noxt : tupl<smn> copy
  auto [nox] = noxt;  // nox : smn lvalue sb

// throwing
  tupl<smx> txxx{}; // txxx : tupl<smx>
  auto xxxt = txxx;    // xxxt : tupl<smx> copy
  auto [xxx] = xxxt;  // xxx : smx lvalue

  static_assert( noexcept(tnox = {}) );
  static_assert( noexcept(tnox = noxt) );
  static_assert( noexcept(tnox = {noxt}) );
  static_assert( noexcept(tnox = std::move(noxt)) );
  static_assert( noexcept(tnox = {nox}) );
  static_assert( noexcept(tnox = {std::move(nox)}) );

  static_assert( ! noexcept(txxx = {}) );
  static_assert( ! noexcept(txxx = xxxt) );
  static_assert( ! noexcept(txxx = {xxxt}) );
  static_assert( ! noexcept(txxx = std::move(xxxt)) );
  static_assert( ! noexcept(txxx = {xxx}) );
  static_assert( ! noexcept(txxx = {std::move(xxx)}) );

  clear(ops);
  tnox = {};
  assert(ops[0] == 3);
  assert(ops[1] == def_cons);
  assert(ops[2] == move_ass);
  assert(ops[3] == destruct);

  clear(ops);
  tnox = noxt;
  assert(ops[0] == 1);
  assert(ops[1] == copy_ass);

  clear(ops);
  tnox = {noxt};
  assert(ops[0] == 1);
  assert(ops[1] == copy_ass);
  static_assert( noexcept(tnox = {noxt}) );

  clear(ops);
  tnox = std::move(noxt);
  assert(ops[0] == 1);
  assert(ops[1] == move_ass);

  clear(ops);
  tnox = {std::move(noxt)};
  assert(ops[0] == 1);
  assert(ops[1] == move_ass);

  clear(ops);
  tnox = {nox};
  assert(ops[0] == 3);
  assert(ops[1] == copy_con);
  assert(ops[2] == move_ass);
  assert(ops[3] == destruct);

  clear(ops);
  tnox = {std::move(nox)};
  assert(ops[0] == 3);
  assert(ops[1] == move_con);
  assert(ops[2] == move_ass);
  assert(ops[3] == destruct);

  tupl<smn,int[4]> ta; // trailing array, non-copyable
  ta = {};

  clear(ops);
  ta = {std::move(nox),{}}; // rvalue array braced-init
  assert(ops[0] == 3);
  assert(ops[1] == move_con);
  assert(ops[2] == move_ass);
  assert(ops[3] == destruct);

  //static_assert( noexcept(ta = tuple{std::move(nox),a}) );
  //static_assert( noexcept(ta = tuple{nox,a}) );

  int arr[4];

  clear(ops);
  assign(ta) = {nox,arr};
  assert(ops[0] == 1);
  assert(ops[1] == copy_ass);

  clear(ops);
  assign(ta) = {std::move(nox),std::move(arr)};
  assert(ops[0] == 1);
  assert(ops[1] == move_ass);

  clear(ops);
  assign_elements(ta,nox,arr);
  assert(ops[0] == 1);
  assert(ops[1] == copy_ass);

  clear(ops);
  assign_elements(ta,std::move(nox),arr);
  assert(ops[0] == 1);
  assert(ops[1] == move_ass);

  clear(ops);
  tupl sta {smn(),tupl{"str"}};
  assert(ops[0] == 1);
  assert(ops[1] == def_cons);

  clear(ops);
  sta.x0 = std::move(nox);
  assert(ops[0] == 1);
  assert(ops[1] == move_ass);

  clear(ops);
  sta = {nox,{"cst"}};
  assert(ops[0] == 3);
  assert(ops[1] == copy_con);
  assert(ops[2] == move_ass);
  assert(ops[3] == destruct);

  clear(ops);
  sta = {std::move(nox),{"cst"}};
  assert(ops[0] == 3);
  assert(ops[1] == move_con);
  assert(ops[2] == move_ass);
  assert(ops[3] == destruct);
}

#include "tupl_vals.hpp"

// test("tupl tie assign")
void tupl_tie_assign()
{
  int i{};
  long l{};
  tie(i,l) = {i,l};
  tie(i,l) = {0,0L};
  tie(i,l) = {0L,0};
  tie(i,l) = {0,l};
  int a2[2];
  tie(a2) = {{i,i}};

  // initialize an array value member from an array lvalue
  constexpr int ca2[2]{1,2};
  constexpr auto arrayinit = tupl(vals<int[2]>{} = {ca2});
  static_assert( arrayinit == tupl<int[2]>{{1,2}} );

  static int ops[16]{};
  auto clear = [](auto&o){for(auto&e:o)e=0;};

  using smn = SMF<true,ops>;

  smn x;
  assert(ops[0] == 1);
  assert(ops[1] == def_cons);


  clear(ops);
  tie(x) = {}; // x = {}
  assert(ops[0] == 3);
  assert(ops[1] == def_cons);
  assert(ops[2] == move_ass);
  assert(ops[3] == destruct);

  clear(ops);
  tie(x) = {x};
  assert(ops[0] == 1);
  assert(ops[1] == copy_ass);

  clear(ops);
  tie(x) = {std::move(x)};
  assert(ops[0] == 1);
  assert(ops[1] == move_ass);

  clear(ops);
  tie(x) = tie(x);
  assert(ops[0] == 1);
  assert(ops[1] == copy_ass);

  clear(ops);
  tie(x) = tie_fwd(std::move(x));
  assert(ops[0] == 1);
  assert(ops[1] == move_ass);

  clear(ops);
  tie_fwd(std::move(x)) = {std::move(x)};
  assert(ops[0] == 1);
  assert(ops[1] == move_ass);

  int a[4];

  clear(ops);
  tie(x,a) = {};
  assert(ops[0] == 3);
  assert(ops[1] == def_cons);
  assert(ops[2] == move_ass);
  assert(ops[3] == destruct);

  clear(ops);
  tie(x,a) = {x,a};
  assert(ops[0] == 1);
  assert(ops[1] ==copy_ass);

  clear(ops);
  tie(x,a) = tie_fwd(std::move(x),a);
  assert(ops[0] == 1);
  assert(ops[1] == move_ass);
}

template <typename T>
constexpr std::add_const_t<T>& as_const(T& t) noexcept { return t; }

bool test_refs()
{
  int i, ii;
  ties<int>{1} = ties<int>{0};
  ties<int&>{i} = ties<int&>{ii};
static_assert( std::is_trivially_copyable_v<ties<int&>> );
 const ties<int&> ti{ i };
  const ties<int&> tii{ ii };
  ti = { 1 };
  tii = ti;
  tie(ii) = tie(i);
  swap(tii,ti);
  swap(tie(ii),tie(i));
  SAME( decltype(ti), decltype(tie(i)) );

  long j, jj;
  char c, cc;
  ties<int&, long&, char&> ijc{ i,j,c }; // non-const
  ties<int&, long&, char&> ijc2{ ii,jj,cc }; // non-const
  swap(ijc, ijc2);

  ijc = {1,2L,'d'};  // assign-through
  auto& [x,y,z] = ijc;
  //ijc = { x, y, z }; // ambiguous with deleted default assignment
  as_const(ijc) = { x, y, z }; // const disambiguates

  assert(x == 1 && y == 2 && z == 'd');
  assert(x == get<0>(ijc) && y == get<1>(ijc) && z == get<2>(ijc));
  assert(&x == &get<0>(ijc) && &y == &get<1>(ijc) && &z == &get<2>(ijc));
  assert((ijc == ties{x, y, z}));

  return true;
};

// test("traits")
void tupl_traits()
{

  enum { aggregate  = 0x01
       , trivial    = 0x02
       , triv_copy  = 0x04
       , assignable = 0x08
       , triv_ass   = 0x10
       };

  auto traits_check
  = []<int v, typename T>()
  {
    constexpr int u = std::is_aggregate_v<T> * aggregate
                    + std::is_trivial_v<T> * trivial
                    + std::is_trivially_copyable_v<T> * triv_copy
                    + std::is_assignable_v<T,T> * assignable;
    static_assert( v == u );
  };

#define TRAITS_CHECK(...) traits_check.template operator()<__VA_ARGS__>()

  TRAITS_CHECK( 0x0f, tupl<> );
  TRAITS_CHECK( 0x0f, tupl<int[1]> );
  TRAITS_CHECK( 0x0f, tupl<int(*)[1]> );
  TRAITS_CHECK( 0x0f, tupl<int(*)()> );

  TRAITS_CHECK( 0x07, tupl<int> const );

//  TRAITS_CHECK( 0x0f,tupl<int&> );
  //TRAITS_CHECK( 0x0f,tupl<int(&)[1]> );
  //TRAITS_CHECK( 0x07,tupl<int const&> );
  //TRAITS_CHECK( 0x0f ,tupl<int&> const );
  //TRAITS_CHECK( ,tupl<int&> const volatile& );
  //TRAITS_CHECK( ,tupl<int&>&& );

  /* Tupl::ish */ //,tupl<int(&)()>
  /* Tupl::ish */ //,tupl<int&&>
  /* Tupl::ish */ //,tupl<int,int&>

    //,0x07
    //,0x07
    //,0x07
    //,0x07
    //,0x07

    //,0x03
    //,0x07
    //,0x03
}

void tupl_get()
{
  using Tupl = tupl<
    char, unsigned char, int8_t, uint8_t, char&, char const&, char&&>;
  static_assert( type_list_indexof<char,Tupl> == 0 );
  //static_assert( type_list_indexof<unsigned char,Tupl> == 1 );
  static_assert( type_list_indexof<int8_t,Tupl> == 2 );
  //static_assert( type_list_indexof<uint8_t,Tupl> == 3 );
  static_assert( type_list_indexof<char&,Tupl> == 4 );
  static_assert( type_list_indexof<char const&,Tupl> == 5 );
  static_assert( type_list_indexof<char&&,Tupl> == 6 );
  //static_assert( type_list_indexof<bool,Tupl> == -1 );

  constexpr tupl chartri{"","1",+"12"};
  using Ch3 = decltype(chartri);

  static_assert( equal_to{}(get<char[1]>(chartri),"") );
  static_assert( equal_to{}(get<char[2]>(chartri),"1") );
  static_assert( equal_to{}(get<const char*>(chartri), chartri.x2) );

  constexpr auto mp0 = tupl_mptr<0,Ch3>;
  static_assert( &(chartri.*mp0) == &chartri.x0 );
  static_assert( &(chartri.*tupl_mptr<2,Ch3>) == &chartri.x2 );

  struct deriv : lupl<int,char> {};
  constexpr auto mp1 = tupl_mptr<1,deriv>;
  static_assert( deriv{0,'1'}.*mp1 == '1' );

  int i;
  using FAT = tupl<int&&,int&,int&&>;
  FAT fat = {1,i,(int&&)i};
  SAME( decltype(get<0>(fat)), int& );
  SAME( decltype(get<0>((FAT&&)fat)), int&& );
  SAME( decltype(get<1>(fat)), int& );
  SAME( decltype(get<1>((FAT&&)fat)), int& );
};

void tupl_swap()
{
  constexpr tupl tA = {1,2L,'\03'};
  constexpr tupl tB = {2,3L,'\04'};

  static_assert( tA == tA && tA != tB && tB == tB );

  auto ta = tA;
  auto tb = tB;

  assert( ta == tA && tb == tB ); // unswapped

  auto maptie = map(ta,[](auto&...a){return tie(a...);});

  auto& [a0,a1,a2] = ta;
  auto& [b0,b1,b2] = tb;

  auto tiea = tie(a0,a1,a2);
  auto tieb = tie(b0,b1,b2);

  //assert( tiea == {1,2L,'3'} ); // == can't have init-list rhs
  assert( equals(tiea, {1,2L,'\03'}) );
  assert( compare3way(tiea, {1,2L,'\03'}) == 0 );
  assert( tiea == maptie );
  assert( tiea < tieb );

  swap(ta,tb); // swap values direct

  assert( tiea > tieb );

  assert( ta == tB && tb == tA ); // swapped

  swap(tiea,tieb); // swap tied values

  assert( ta == tA && tb == tB ); // unswapped

  assert( equals(ta, tA) );
  assert( equals(ta, {1,2L,'\03'}) );
  assert( equals(ta, {1,2,3}) );
  assert( compare3way(ta, {1,2,3}) == 0 );
  assert( compare3way(tiea, {1,2,3}) == 0 );

  tupl noxt = {'\03'};
  assert( equals(noxt, {3}) );
  tiea = {1,2L,4};
};

void tupl_swap_move_only()
{
  tupl ta = {std::make_unique<char>('A')};
  tupl tb = {std::make_unique<char>('B')};

  swap(ta,tb);

  assert( *ta.x0 == 'B' && *tb.x0 == 'A' ); // swapped
};

void tupl_cats()
{
  constexpr int i12[2]{1,2};
  // copy initialize tupl from array lvalue, using roundabout method
  constexpr tupl ci12 = tupl_init(i12);
  //constexpr tupl cc12 = tupl_init<tupl,int[2]>({1,2});
  //static_assert( ci12 == cc12 );

  constexpr auto ce = cat(tupl{});
  constexpr auto c0 = cat<tupl>();
  constexpr auto ci = cat(ci12);
  constexpr auto ci0 = cat(ci12,c0);
  constexpr auto cici = cat(ci12,ce,ci12);

  static_assert(tupl_size<decltype(ce)> == 0);
  static_assert(tupl_size<decltype(ci)> == 1);
  static_assert(tupl_size<decltype(ci0)> == 1);
  static_assert(tupl_size<decltype(cici)> == 2);

  // copy initialize tupl from array lvalue, using cat
  constexpr auto t12 = cat<tupl>(ties{i12});

  static_assert( t12 == ci12 );
  SAME( decltype(t12), decltype(ci12) );
  SAME( decltype(t12), tupl<int[2]> const );


  constexpr tupl abc = cat(tupl{'a'},tupl{"bc"});

  static_assert( sizeof abc == 4 );
  static_assert( abc == tupl{'a',"bc"} );
  static_assert( __builtin_bit_cast(tupl<char[4]>,abc) == tupl{"abc"});

  constexpr tupl tupl8 = cat(tupl{1},tupl{2},tupl{3},tupl{4}
                                 ,tupl{5},tupl{6},tupl{7},tupl{8});
  static_assert( tupl8 == tupl{1,2,3,4,5,6,7,8});

  constexpr tupl ch16 = cat(tupl{'0'},tupl{'1'},tupl{'2'},tupl{'3'}
                                ,tupl{'4'},tupl{'5'},tupl{'6'},tupl{'7'}
                                ,tupl{'8'},tupl{'9'},tupl{'a'},tupl{'b'}
                                ,tupl{'c'},tupl{'d'},tupl{'e'},tupl{'f'}
                                );
  static_assert( ch16 == tupl{'0','1','2','3','4','5','6','7'
                             ,'8','9','a','b','c','d','e','f'});

  constexpr auto c1 = cat(tupl{}, tupl{0}, tupl{1,2});
  static_assert( c1 == tupl{0,1,2} );

  constexpr auto c3 = cat(tupl<int[1]>{1}, tupl<int[2]>{2,3},
                          tupl<int[3]>{4,5,6});
  static_assert( c3 == tupl<int[1],int[2],int[3]>{{1},{2,3},{4,5,6}} );

  tupl ta = {std::make_unique<char>('A')};
  tupl tb = {std::make_unique<char>('B')};

  auto tab = cat<tupl>(std::move(ta),std::move(tb));

  SAME( decltype(tab), tupl<std::unique_ptr<char>,std::unique_ptr<char>> );
  assert( *tab.x0 == 'A' && *tab.x1 == 'B' );

  char str[] = "str";
  auto stt = cat<tupl>(ties{"str",str}); // reference -> value copy
  SAME( decltype(stt), tupl<char[4],char[4]> );

  auto ttt = cat<ties>(stt, tupl{"str"}, ties{"str",str});
  //wotype<decltype(ttt)> eh;
  SAME( decltype(ttt), ties<
    char(&)[4],char(&)[4],  char(&&)[4],  char const(&)[4], char(&)[4]> );
};

int main()
{
  char a[4]; int v;
  tie(a,v) = {"c++",20};
  tie(a,v) = {a,20};

  tupl<int[1],int> t = {{1},1};
  t = {{1},1};

  tupl{0,0,tupl{0,0}} = {1,2,{3,4}};

  auto t2 = tupl{0,0};
  auto t4 = tupl{0,0,tupl{0,0}};

  t2 = {3,4};
  t4 = {1,2,{3,4}};

  tupl_val_assign();
  tupl_assign_SMFops();
  tupl_tie_assign();
  tupl_categories();
  tupl_sizes();
  tupl_traits();
  tupl_get();
  tupl_swap();
  tupl_swap_move_only();
  tupl_cats();
}
