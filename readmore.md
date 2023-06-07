# Appendices

If you just want to use `tupl` then you can safely skip these appendices.  
The [`readme`](readme.md) covers most important take-aways with its code snippets.  
Here, implementation details and their edge-cases are detailed, and  
there's a discussion of design decisions.

* [Summary](#summary)
* [Initialization](#initialization)
  * [Braced init-lists](#braced-initializer-lists), [CTAD](#ctad), [Aggregate initialization](#aggregate-initialization),  
[Complete initialization](#complete-initialization), [Special Member Functions](#special-member-functions),  
[Array members](#array-members), [Derived `tupl`s](#derived-tupls), [Structural `tupl`s](#structural-tupls),  
[C++20](#c20)
* [Assignments](#assignments)
  * [Assignments summary](#assignments-summary)
  * [Self assignment](#self-assignment), [Generic assignment](#generic-assignment),  
  [Builtin assignment](#builtin-assignment), [Special assignment](#special-assignment),  
  [Aggregate assignment](#aggregate-assignment), [List assignment](#list-assignment),  
  [Custom assignment](#custom-assignment)
* [`tupl` Design Notes](#tupl-design-notes)

If C arrays were regular types and if C++ implemented 'assign-through'  
for reference members then this extra material wouldn't be necessary.

## Summary

For `tupl`s of regular value types, initialization and assignment all works  
well enough using purely builtin facilities of struct aggregates.

In the presence of irregular types some builtin operators fail
and code is  
then needed to implement those operations (if the semantics is sound).

The presence of reference elements calls for library code to implement  
assignments; `tupl`-derived type `ties` adds `operator=` overloads that  
emulate the semantics of the equivalent value tupl for all-reference ties.

The presence of array elements is a problem for aggregate initialization;  
if the list contains an array-valued initializer then copy initialization fails.  
Array member initialization could be addressed by adding constructors.  
Constructors are antithetical to `tupl` as a minimal language-level type,  
so a workaround is provided instead - a `tupl_init` 'maker function'.

`tupl` initialization take-aways:

* Initialization from braced lists is idiomatic
* `tupl` initialization is aggregate initialization
  * or SMF copy-or-move initialization from same-type tupl
  * or derive from tupl add custom constructors.
* Aggregate initialization of array from array doesn't work;  
  `tupl_init` 'maker function' initializes array members.

`tupl` assignment take-aways:

* `tupl` has implicit builtin SMF default assignments only.
* `tupl` assignment from braced initializer lists is encouraged.
* `ties` derived tupl provides specialized `operator=` overloads  
for tupls of reference types, from tuplish types or braced inits.
* `assign_elements` does efficient elementwise assignments.
* `assign` assigns from other tuplish types or braced inits.

`tupl` is straightforward to use, once its aggregate nature is accepted.  

## Initialization

Any discussion of initialization in C++ gets involved...

### Braced initializer lists

Lists of values enclosed in curly braces are a natural tuple representation.

Lists of all same-type values can be seen as array-valued literals  
(`std::initializer_list` eagerly interprets them as arrays).

### CTAD

`tupl` [CTAD](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)
deduces types from a braced initializer list then tailor-makes  
a struct with those member types
ready for direct initialization by the list.

There's a potential pitfall with nested `tupl` deduction; CTAD collapses  
a single tupl argument rather than deducing a nested tupl:

```c++
  tupl t{v...}; // tupl<T...>
  tupl{t};      // tupl<T...> 'collapse'!
  tupl{t,t}; // tupl<tupl<T...>,tupl<T...>>
```

`tupl` CTAD deduction guides don't attempt to fix this  
(following the principle of least surprise) and, similarly,  
`tupl` CTAD doesn't recursively descend into nested braces.

### Aggregate initialization

With no constructors, and no user-declared special member functions,  
`tupl` follows purely builtin language rules for initialization.

[Aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization)
is "the original and the best" `tupl` constructor.

The [next section](#assignments) shows how aggregate initialization
is also the first step  
in assigning to `tupl`s from braced initializer lists.

Unfortunately, aggregate initialization fails for
array-valued initializers  
so a 'maker function' workaround is needed to support C arrays:

* `tupl_init` constructs `tupl` elements from its argument list  
exploding any array values into individual element initializers  
(using integer_sequence expansion and a brace-elided init list).

* `tupl_cat` delegates to `tupl_init` to handle array members.

See [array members](#array-members) below for more detail.

### Designated initializers

The default member ids are numbered, `x0, x1, x2...`  
and can be used as designators to help match initializers with elements.

### Complete initialization

Aggregate initialization is lax:

* It allows incomplete initialization
* It allows to skip braces for nested aggregate members

Of course, it's best to initialize all values and include all nested braces;  
missing elements can be a bug and skipped braces are confusing.

Most compilers provide warnings for missing initializers or braces.  
Aggregate initialization is as safe as an equivalent generic constructor  
if warnings are enabled (and read, acted on, and removed).

(This library relies on brace-elided aggregate initialization in two places  
and silences warnings.)

`tupl_init` enforces complete initialization.

### Special member functions

Aggregate initialization is backed up by the
[special member functions](https://howardhinnant.github.io/bloomberg_2016.pdf#page=30);  
implicitly-defaulted copy or move operations, synthesized automatically  
as needed when the element types allow it.

* Special member functions all just work correctly for `tupl`'s of  
all
[regular value type](https://en.cppreference.com/w/cpp/concepts/regular)
members, as well as for arrays of them:
  * default constructor (and destructor)
  * copy and move constructors
  * copy and move assignments

If the element types don't support a SMF then it isn't auto-synthesized  
and is instead implicitly defined as deleted
to give a compile error if used  
(or [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)).

For instance, a `tupl` is default constructible when all its elements are,  
or, vice versa, presence of non default constructible elements disables it.  

* Reference members disable the default constructor and both  
assignment operators,
but a copy constructor *is* synthesized  
(to copy the pointer value underlying the reference).

E.g, `tie`s are not default constructible since reference members *require*  
an initializer (while regular types are required to be default constructible).

### Array members

Arrays are not regular types on their own because they're non-copyable.  
As struct members they become regular; the special member functions  
take care of copying all elements by default, array members included.

However...

* Arrays cannot be copy-initialized from array-valued initializers $^*$

* Array declarations are initialized by element initializer lists only $^*$

* Arrays can't be assigned-to directly, not even from initializer lists

$^*$ String-literals are a special case; they can copy-initialize char arrays.  
String literals are overused in the snippets as convenient array literals.

```c++
  tupl cpp20{"c++",20}; // string literal works

  constexpr char cpp[]{"c++"}; // array variable
//tupl cpp20{cpp,20}; // array variable: FAILS

  using chars = char[];
//tupl cpp20{chars{"c++"},20}; // rvalue: FAILS
```

In general, elementwise initialization is the only way to initialize arrays.  
The initializer list can be empty or incomplete though, in which case any  
trailing elements without initializers are value initialized.

The `tupl_init` 'maker function' is a workaround for initializing arrays:

```c++
  tupl cppstd = tupl_init(cpp,20); // maker works
  lupl stdcpp = tupl_init<lupl>(cpp,20); // lupl

  tupl_init<tupl,char[4],int>(cpp,20); // explicit

  // Here's the equivalent exploded initializer list
  // (brace elision may give missing-braces warning)
  tupl<char[4],int>{cpp[0],cpp[1],cpp[2],cpp[3],20};
```

`tupl_init` forwards all its arguments to aggregate-initialize a tupl type.  
A template argument can be given for the template id alone, in which case  
CTAD is emulated, or for the template id plus all the element types.

Any array value initializers are exploded into individual element initializers  
that are then used to aggregate-initialize the tupl via brace elision.

### Derived `tupl`s

Types derived from `tupl` can add what they want, constructors included.  
An example of a derived tupl with an added constructor is given next.

All the provided derived `tuplish` types
remain aggregate types because,  
as generic utilities, they have no invariants.
Constructors add nothing here  
but take away the convenient propagation of element properties.

### Structural `tupl`s

Types that can be used for non type template parameters are 'structural'.  
A `tupl` is structural if its elements are structural (or arrays of structural).  

For example, a `tupl` can act as a 'static string' template parameter:

```c++
  template <tupl s>
    requires cstring_tupl<s> // see below
  struct tring : decltype(s) {
    consteval tring() : decltype(s)(s) {}
  };
```

`tring` derives from the `tupl` argument's type and adds a default  
constructor to initialize its `tupl` base with the argument value  
(or it could be an empty class with no constructor and static access only).

Note that the '`tupl`' template id itself is used as placeholder parameter;  
the parameter can be CTAD-deduced from a braced initializer argument  
(Clang doesn't accept the braced init yet, so requires the full type):

```c++
  using dmr = tring<{"DMR"}>; // NTTP CTAD

  using rip = tring<tupl{"DMR"}>; // explicit

  static_assert( same_as<dmr,rip> );
  static_assert( dmr{} == tupl{"DMR"} );
```

Notice how the string value is transported in the type alone.

The `cstring_tupl` concept can be coded as below, with `ntbs` helper  
(which searches for any nul character, not necessarily at the array's end):

```c++
  template <c_array<char> A>
  consteval bool ntbs(A const& a) {
    for (auto& c : a)
      if (c == 0)
        return true;
    return false;
  }

  template <tupl s>
  concept cstring_tupl
                = tupl_size_v<decltype(s)> == 1
                  && ntbs(s.x0))
```

### C++20

C++20 allows parenthesized initialization of aggregates, so generic code  
can use the same constructor syntax for all types without special-casing  
(as long as all element types are regular).
Paren-init admits array decay  
and narrowing conversions; always prefer braces for non-generic uses.

C++20 also introduced default CTAD for aggregates.
However, it `auto`-  
deduces types by 'decay copy' rules. `tupl` adds a user-defined  guide to  
deduce value types for all arguments, including arrays, with no decay.

GCC and MSVC accept braced init-lists as template parameters.  
We hope that this convenient feature will be accepted as a C++20 DR  
and implemented in Clang.

## Assignments

`tupl` itself is sufficient for dealing with regular value types.

For regular `tupl`s, the builtin default `operator=` assignments do their job  
to move or copy assign all elements correctly from `rhs` to `lhs`.

(Learning the `tupl` assignment 'API' is learning about builtin assignment.)

For tupls of references, the assignment operators are deleted so the library  
steps in with a derived tupl type `ties` that implements all assignments as if  
it were the equivalent tupl of values.
Implementing list assignment calls for  
a design decision on how closely to emulate aggregate assignment - `ties`  
forfeits the ability to independently move or copy individual elements in a  
list assignment (this choice eliminates the need for temporary copies).

`tupl` lacks 'heterogeneous' assignments, like `std::tuple` has, to assign  
from other tuplish types.
These are useful so `ties` gets them too.

Extended assignment operators can also be useful for value tupls, so the  
library provides a derived tupl type `vals` with heterogeneous assignments  
and list assignments (to fix the case of array-valued list items).

These overloads are the only non-static member functions of the library!

The "`tupl_tie.hpp`" header is required for `ties` and `tie` support.  
The "`tupl_val.hpp`" header defines `vals`.  

### Assignments summary

`tupl`s of regular value types are regular  
(plus, array members are made regular).  
Regular types require copyability.

Copyability requires assignments:

```c++
                    lhs = rhs
```

where `rhs` is of the same type as `lhs`.  

The rhs of an assignment can be a braced initializer list:

```c++
                    lhs = {v...}
```

In this case the list is used to initialize a temporary of `lhs` type that is then  
move assigned to `lhs` before being destroyed.
Or, custom list assignment  
can be implemented more efficiently with a concrete 'view' type argument  
that passes the RHS initializer list items by-reference rather than by-value.

The empty brace case gives nice syntax for 'clearing' all elements of a tupl:

```c++
                    lhs = {}
```

Rhe `ties` type is used for reference tupls.
It replaces the deleted default  
assignments with custom assignments, including for list assignment and  
empty brace assignment:

* `ties` derives from tupl and adds five `operator=` overloads:
  1. templated replacement for SMF assignments
  1. templated other-tuplish assignments
  1. concrete list move assignment
  1. concrete list copy assignment
  1. concrete empty list assignment

(concrete argument type overloads are templated for overload resolution).

Some of these extra assignment operators are useful for value tupls too:

* `vals` derived value-type tupl adds three `operator=` overloads:
  1. templated other-tuplish assignments
  1. concrete list move assignment
  1. concrete list copy assignment

(the list assignments work for array-valued list items).

The custom list assignments eliminate the use of temporaries (inherent in  
aggregate assignment), but in doing so lose the ability to independently  
move or copy from individual list items.

'Forwarding assignment', i.e. elementwise move or copy assignment, can  
be done by the `operator=`  overload that assigns from other tuplish types  
by assigning from an RHS tupl of mixed rvalue and lvalue references.

`std::tuple` has such 'heterogeneous' assignments, so having them here  
can also help in porting code from tuple to tupl.

c.f.
[operator assignment](https://en.cppreference.com/w/cpp/language/operator_assignment),
[assignment operator](https://en.cppreference.com/w/cpp/language/operators#Assignment_operator),
[copy](https://en.cppreference.com/w/cpp/language/copy_assignment) /
[move assignment](https://en.cppreference.com/w/cpp/language/move_assignment).

### Self assignment

If all element types self assign correctly then `tupl` self assigns correctly.

### Generic assignment

Assignment in C++ is reliant on '`=`' syntax.
There's no generic facility to use  
non-operator syntax for assignments (like `std::less` for comparisons).  
The need arises is in supporting C array assignment.

Generic assignment facilities are provided in `c_array_support` library:

* `assign(l)`
* `assign_elements(l,e...)`

In `tupl` library, these functions are overloaded to assign to tuplish types,  
and they're used to implement the custom `operator=` overloads below.

### Builtin assignment

`tupl`, as an aggregate with no `operator=` overloads, relies on the builtin  
means of assignment only; default synthesized copy and move assignment.

Builtin assignment, on its own, works well enough for regular `tupl`s.

('Well enough' because assignment from braced initializer lists `={...}`  
brings a couple of caveats - see 'aggregate assignment' below).

### Special assignment

Copy and move assignments from an rhs operand of the same type as lhs  
are usually handled by `operator=` special member functions.

The default synthesized operators work well enough for value type `tupl`s  
(as covered above).

For `tupl`s of reference types, custom assignment operators are needed;  
`ties::operator=(Ties&&)` handles both move and copy assignments  
(and `ties` remains trivially copyable because this templated overload is  
not recognised as an actual SMF signature).

### Aggregate assignment

Struct aggregates of regular value type data members can be assigned to  
from braced lists of values.

'Aggregate assignment' is used here specifically for such assignments:

* to an aggregate struct
* from a braced initializer list
* *and* using the builtin move assignment operator.

There are two caveats:

1. Aggregate assignment of array values from array variables fails  
in the attempt to aggregate initialize the RHS temporary  
(array values can only be assigned elementwise).
2. Aggregate assignment of non-trivial types creates and destroys  
temporaries, so is not optimal.

These edge cases are dealt with by custom assignment operators in derived  
tupl types `ties` and `vals`.

Despite these caveats this library promotes list assignment `t = {v...}`  
as a convenient syntax for assigning to tupl elements from lists of values.

Aggregate assignment is efficient for trivial `tupl`'s.

```c++
  tupl<char[4],int> cppstd; // uninitialized

  cppstd = {"c++",20}; // 'aggregate assignments'

  cppstd = {};      // clear all elements = {}
  cppstd = {{'c'}}; // array braced-init & assign
              // ^ may warn 'missing init' for int
```

This is in fact aggregate initialization followed by move-assignment:

```c++
  cppstd = tupl<char[4],int>{"c++",20}; // same
```

The sequence of initialization followed by move-assignment can cause  
temporaries to be created for non trivial elements:

1. RHS temporaries are constructed
2. the temporaries are move-assigned from
3. the temporaries are destroyed

```c++
  tupl<std::string,int> str_int{"c++",20};

  std::string moveme = "moveme";
  str_int = {moveme};            // copy then move
  str_int = {std::move(moveme)}; // move then move
```

For trivial element types any copies are elided and codegen is good.  
For non trivial types, the alternative means of assignment covered below  
eliminate the temporary copies and redundant moves.

### List assignment

'List assignment' is the general term for any assignment from a braced list.  
The target may be a non-aggregate class or the assignment may be done  
with a user-defined custom `operator=` overload, or both, or neither.

E.g. [`std::initializer_list`](https://en.cppreference.com/w/cpp/utility/initializer_list)
is intended for use as an argument type  
for list assignments from 'homogeneous' lists of all-same type values  
(this was part of a push for 'uniform initialization' syntax in C++11).

For non-aggregate classes, their non-explicit constructors are considered.  
If constructor arguments match the values in the initializer list then they're  
candidates to list initialize an RHS temporary to be move-assigned from  
(this constructor route may be ambiguous with `operator=` candidates).

E.g. list assignment syntax works for `std::tuple` in a very similar way to  
how aggregate assignment works for `tupl`.
On the other hand, for `ties`,  
a custom `operator=` is needed:

```c++
#include <tuple>
#include "tupl"
#include "tupl_tie"

void list_assign(std::tuple<int,char>& t)
{
  t = {1,'a'}; // List assignment via constructor
}

void list_assign(lml::tupl<int,char>& t)
{
  t = {1,'a'}; // Aggregate assignment
}

void list_assign(lml::ties<int&,char&> t)
{
  t = {1,'a'}; // List assignment; custom operator=
}
```

Here, `std::tuple` employs a constructor for the RHS list while `tupl` is  
aggregate initialized. In both cases the list initializes an rvalue temporary  
on the rhs, of the same type as the lhs, and it is move assigned to the lhs  
by the default synthesized operator, and then the temporary is destroyed.

The `ties` case is covered next; no temporaries are created or destroyed  
in performing this assignment.

#### Design dilemma

Custom list assignment can eliminate the use of temporary copies, but only  
by forfeiting the ability to copy or move from  each list value independently.

In implementing a list assignment `operator=(BIL)`, the argument type  
`BIL` must be provided as a concrete type to match its braced initilizer list.  
While homoegenous lists can be deduced as `std::initializer_list`  
(or as a C array) `BIL` can't be deduced in general.

Deduction is required for argument forwarding.
In the absence of forwarding,  
list assignment with mixed individual copy or move  semantics is only feasible  
with by-value `BIL` element types that 'stage' temporary copies to move from.

Ideally, `BIL` should pass by-reference (or cheap-to-copy trivial value types).  
It isn't feasible to add overloads for all rvalue or lvalue element combinations.  
It might seem that reference members have to be all rvalue-references,
to be  
moved from only. An all-copy overload might seem 'semantically challenged'  
as it copies values from an rvalue list.

In fact, even though the list itself is an rvalue, reference-collapse sees that any  
reference members retain their member type reference 'category' on access -  
rvalue or lvalue - to be moved from or or copied from.

So the all-copy case appears kosher too.
Move-only types have to be moved.  
Once one is moved, all have to be moved.
Moving none will copy all.

`ties` and `vals` implement both all-move and all-copy list assignments, with  
overload resolution preferring the all-move case.

### Custom assignment

Custom assignment is most obviously needed for tupls of references, which  
otherwise have no working assignments.

Then, user `operator=` overloads are the only way to assign from non-same  
types (assuming that implicit conversions are not in play).

They're also needed to 'fix' list assignment edge cases,
like not being able to  
assign from array-valued list items.

`tupl` itself has no `operator=` overloads
so in order to use '`=`' to do any of  
these assignments you have to derive from tupl and add custom operators  
(or, otherwise, just use the generic free functions for assignment instead).

The main idea of the `ties` type is allow reference tupls to accept the same  
assignments as value tupls.
Heterogenous assignments are added in too.

A comparison with `std::tie` assignment is instructive:

```c++
void tie_assign(int i, char c)
{
  lml::tie(i,c) = {1,'a'}; // works: custom =

//std::tie(i,c) = {1,'a'}; // FAIL: no viable =

  std::tie(i,c) = {i,c}; // list assignment (works!)
}
```

`lml::tie` accepts assignment from lists of values, by design.  
This is a significant difference when compared to `std::tie`.

`std::tie` list assignment from values fails as there's no `operator=`  
defined for it (but list assignment from lvalues works as an unintended  
consequence of construction followed by move-assignment).

Note that `ties::operator=` overloads don't make it non-aggregate  
(unlike initialization where adding constructors makes it non aggregate).

## `tupl` design notes

`tupl` aims to explore what a language tuple might look like.  
It works with, and not against, builtin language mechanics.  
Its aggregate nature constrains and shapes its design decisions.

It satisfies most use cases, with no non-static member functions.  
The `ties` and `vals` types derive from `tupl` and add `operator=`  
overloads - the only non-static member functions of the library.

Assignment and comparison operators accept same-types only.  
This falls out from the use of builtin operations for assignments  
and the use of default synthesized `operator<=>` when possible.

Free functions are provided to fill in where class operators fail.

Alternatively, `tupl` is designed to be derived from and extended.  
The `ties` type is the main example, providing custom assignment  
for reference-tuples which otherwise have no builtin assignment.

`std::tuple` sets API precedence, and has bred familiarity.  
"Compatible as appropriate; not inappropriately incompatible"  
captures `tupl`'s take from `tuple`; not deliberately different,  
but deliberatively designed towards what is most *appropriate*  
for a language-level tuple.

For [**`std::tuple`**](https://en.cppreference.com/w/cpp/utility/tuple)
to be implementable as a library type it needed  
variadic packs to replace macro expansions of earlier Boost tuple.

The `tupl` implementation is based on old-style macro expansions,  
to generate specializations for each arity up to some fixed maximum.  
This can be replaced by member pack expansion when the feature is  
available in the language (or as a compiler extension).

Look past the macro-based implementation and focus on design.  
`tupl` explores what is possible with a single struct aggregate.  
What design is most appropriate for a language tuple?

**`tupl`** is aggregate so it automatically propagates trivality.

* `tuple` has 26 constructors and 12 overloaded `operator=`.  
* `tupl` has no constructors and no assignment operators.

`tupl` initialization and assignment are builtin operations.

`tuple` allows initialization and assignments from different tuple types.

* `tuple` will implicitly convert element types with no warnings.
* `tupl` aggregate semantics admits only 'uniform' non-narrowing  
conversions; compilers warn of conversions or missing values.

`tuple` has heterogeneous comparison operators
(different tuple types  
can be compared as long as they have all comparable element types).  
`tuple` doesn't provide 3-way comparison of different-length tuples.

* `tupl` comparison operators only compare same-type `tupl`s.
* General heterogenous comparison is done by named functions;
  * `compare3way(l,r)` admits different-length `tupl` types
  * `equals(l,r)` allows 'converting comparisons' like tuple
* Or by the `cmps` type derived from `tupl` with specialized `<=>`

`std::tuple` assignment operators handle both value and reference  
members together. This use case is only supported by the free function  
assignments for `tupl` types.

`tupl` is 'opinionated' in only supporting `operator=` assignments for  
either all-value or all-reference tupls.

* `tupl` is intended to hold values only
* `ties` is intended for reference tupls only

`std::tuple` is hostile to C array as an element type.

`tupl` doesn't subscribe to the `std` protocol for tuple-like types.  
Structured binding is provided directly by struct aggregate binding.

`tupl` can give application-level speedup of 5% or more.

ToDo: provide benchmark
