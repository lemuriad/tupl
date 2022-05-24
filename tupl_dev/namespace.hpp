#ifndef NAMESPACE_ID     // NAMESPACE_ID = your name for the namespace
#define NAMESPACE_ID lml // Configure with a -DNAMESPACE_ID=ns define
#endif                   // (an empty def inhibits namespace generation)

#define VA_ARG1(A0,A1,...) A1
#define VA_EMPTY(...) VA_ARG1(__VA_OPT__(,)0,1,) // (Detects __VA_OPT__)

#if (VA_EMPTY(?))     // VA_EMPTY(?) true if __VA_OPT__ is not supported
static_assert( false, // Informative warn if __VA_OPT__ is not supported
  "Preprocessing requires __VA_OPT__ support. "
  "MSVC may need /Zc:preprocessor switch");
#endif

#if ! VA_EMPTY(NAMESPACE_ID)
#  ifndef NAMESPACE_IS_OPEN
#  define NAMESPACE_IS_OPEN
namespace NAMESPACE_ID {  // OPEN  namespace NAMESPACE_ID
#  else
}                         // CLOSE namespace NAMESPACE_ID
#  undef NAMESPACE_IS_OPEN
#  endif
#endif

#undef VA_EMPTY
#undef VA_ARG1
