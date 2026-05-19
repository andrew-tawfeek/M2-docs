// (c) 1997 Michael E. Stillman

#ifndef _error_h_
#define _error_h_

/**
 * @file error.h
 * @brief Engine error-reporting primitives: `ERROR`, `INTERNAL_ERROR`, `error`, `error_message`.
 *
 * Declares the C-linkage error API used throughout the engine.
 * `ERROR(fmt, ...)` is the printf-style entry point that sets a
 * thread-local error string and an "we have an error" flag;
 * `error()` queries that flag, `error_message()` reads the string
 * back out, and `INTERNAL_ERROR` is the unrecoverable variant that
 * exits the program. The header `#undef`s a stray `ERROR` macro
 * that some MinGW system headers define as `0`.
 *
 * The engine avoids C++ exceptions across the boundary in favour
 * of this protocol: a `.cpp` routine calls `ERROR(...)` on a
 * detected problem and returns; every caller checks the flag and
 * propagates an early exit; the interpreter loop ultimately
 * surfaces the message to the M2 user. C linkage is deliberate so
 * the routines remain callable from the `.d`-generated glue, the
 * C++ engine code, and external library callbacks alike. The
 * thread-local state matters for the `Computation` workers run
 * under the system supervisor --- each worker has its own error
 * slot.
 *
 * @see buffer.hpp
 * @see overflow.hpp
 */

#if defined(__cplusplus)
extern "C" {
#endif

  // under mingw32 an include file defines ERROR as 0
  #undef ERROR

  void ERROR(const char *s,...);
  void INTERNAL_ERROR(const char *s, ...); /* Exits the program with an error code */
  int error(); /* returns 0 for false, 1 for true */
  const char *error_message();

#if defined(__cplusplus)
}
#endif

#endif

/*
// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
*/
