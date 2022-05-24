# `tupl`

**`tupl`** is a baggage-free reboot of a tuple type for C++20 with a goal of lowest
[abstraction penalty](
  https://en.wikipedia.org/wiki/High-level_programming_language#Abstraction_penalty).  
Imagine if C++ had tuples built in to the language...

* **`tupl<T...>`** is a struct aggregate with the type pack `T...`
expanded as members, therefore:

* If `T...` are trivial, standard-layout and / or structural types  
then `tupl<T...>` is a trivial, standard-layout and / or structural type

These properties are propagated by virtue of being a struct aggregate.  
In particular, a `tupl` is
[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable)
when all its elements are,  
and this makes it cheap to pass to and return from functions

```c++
    tupl tup = { 1, 2U, "3" }; // CTAD deduces:
                               //
    using Tup = decltype(tup); // tupl<int, unsigned, char[2]>

    template <auto> struct is_structural : true_type {};

    static_assert( is_trivially_copyable< Tup >()
                &&    is_standard_layout< Tup >()
                &&          is_aggregate< Tup >()
                &&            is_trivial< Tup >()
                &&         is_structural< Tup{} >() );
```

**`tupl`** layout has `[[no_unique_address]]` attribute on all members.  
It is layout compatible with an equivalent struct aggregate:

```c++
    struct Agg { [[no_unique_address]] int i;
                 [[no_unique_address]] unsigned u;
                 [[no_unique_address]] char c[2]; };

    static_assert( is_layout_compatible< Tup, Agg >() );
```

As provided, `tupl` supports up to 16 elements with hex-digits as member ids:

```c++
    x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, xa, xb, xc, xd, xe, xf
```

A preprocessor codegen step allows to reconfigure this `tupl` 'max_arity'.  
Elements may be accessed by member id, or by `get`, or by the new `getie`:

```c++
  &tup.x2 == &get<2>(tup); // address the char[2] element of tup
  get<0x2>(tup) = "3";     // error: cannot assign array to array
  getie<2>(tup) = {"3"};   // assign-to the char[2] element
```

Here, `getie<2>(tup)` combines `get` and `tie` as `tie(get<2>(tup))`
returning `tupl<char(&)[2]>`  
and the resulting reference tupl supports array assignment
via an `operator=` overload.

Assignment `operator=` is the only non-static member function.
It is defined only when needed to work around lack of language support
for assigning through reference members and for assigning to arrays.

## Dependencies

C++ standard library:

* `<concepts>` for `assignable_from`, `ranges::swap` CPO, etc.  
* `<compare>` for three-way `operator<=>` comparisons, etc.  

Support library, factored out:

* [`c_array_support`](https://github.com/willwray/c_array_support)
project for generic support of array
members:  
comparison and assignment of arrays alongside other regular types.
  * `<cstdint>`, required on MSVC only, for `uintptr_t`

Preprocessor library, for code generation (optional):

* [`IREPEAT`](https://github.com/willwray/IREPEAT):
C++ preprocessor macros for indexed repetitions

Tested on recent GCC>=10, Clang and MSVC compilers.

## Meson build

A minimal meson.build is provided. CMake is coming.

As a header-only library no build is necessary, as such,
but a 'build' script is very useful:

* to fetch the dependencies and marshall the header locations
* to specify build flags, options and optional targets
* to run the code generation step
* to provide a test runner

```bash
  git clone https://github.com/Lemurian-Labs/tupl.git
  cd tupl
  meson setup build
  meson compile -C build
  meson test sanity -C build
```
