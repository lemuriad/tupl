/*
 SPDX-FileCopyrightText: 2023 The Lemuriad <opensource@lemurianlabs.com>
 SPDX-License-Identifier: BSL-1.0
 Repo: https://github.com/Lemuriad/tupl
*/

// "tupl_platform.hpp" platform-specific macros for portability

#if ! defined(UNREACHABLE) && ! defined(NUA)

// UNREACHABLE: platform-specific annotation for unreachable code
// till there's a portable or standardized annotation (c.f. P2390 P0627)
#ifndef UNREACHABLE
#  ifdef __GNUC__
#    define UNREACHABLE() __builtin_unreachable();
#  elif defined(_MSC_VER)
#    define UNREACHABLE() __assume(false);
#  else
#    define UNREACHABLE() // empty def; there may be errors or warnings
#  endif
#endif

// NUA no_unique_address attribute, platform specific handling for MSVC
#ifndef NUA
#  ifdef __has_cpp_attribute
#    if __has_cpp_attribute(no_unique_address)
#      define NUA [[no_unique_address]]
#    endif
#  endif
#endif
//
// no_unique_address: platform workaround for MSVC >= VS2019 v16.10
// (_MSC_FULL_VER >= 192829913 for VS2019 v16.9)
#if !defined(NUA) && _MSC_VER >= 1929
#  define NUA [[msvc::no_unique_address]]
#endif
//
#if !defined(NUA)
static_assert(false,"[[no_unique_address]] attribute is required");
#endif

// INT_SEQ_MAP & SEQ_MAP wrap integer-sequence generator builtins
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

// type_pack_element_t; Clang has a builtin, use it if available
//
#ifdef __has_builtin
#if __has_builtin(__type_pack_element)
#define TYPEPACKEL __type_pack_element<I,E...> // I,E... are 'baked in'
#endif
#endif

// Supress noisy missing-braces warnings on Clang
//
#ifndef NO_WARN_MISSING_BRACES
# if defined(__clang__)
#  define NO_WARN_MISSING_BRACES(...)\
_Pragma("GCC diagnostic push")\
_Pragma("GCC diagnostic ignored \"-Wmissing-braces\"")\
__VA_ARGS__ \
_Pragma("GCC diagnostic pop")
# else
#  define NO_WARN_MISSING_BRACES(...)__VA_ARGS__
# endif
#endif

#else // UNDEFINE

#undef NO_WARN_MISSING_BRACES
#undef TYPEPACKEL
#undef SEQ_MAP
#undef INT_SEQ_MAP
#undef NUA
#undef UNREACHABLE

#endif
