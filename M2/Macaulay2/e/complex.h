// Copyright 2008  Michael E. Stillman

#ifndef _complex_h_
#define _complex_h_

/**
 * @file complex.h
 * @brief `gmp_CC` C primitives: arithmetic on arbitrary-precision complex values (pair of MPFR reals).
 *
 * Declares the engine's C-style API for `gmp_CC` --- an
 * arbitrary-precision complex number stored as a pair of MPFR floats.
 * The function naming follows MPFR/GMP conventions
 * (`mpfc_init` / `mpfc_init_set` / `mpfc_clear` /
 * `mpfc_add` / `mpfc_mul` / `mpfc_div`, plus special functions like
 * `exp`, `log`, `sqrt`), the result is always the first argument,
 * and every rounding step uses `MPFR_RNDN`. Every `gmp_CC` must be
 * initialised with `init` (or `init_set`) before use and freed with
 * `clear`.
 *
 * The C surface exists because the interpreter / engine boundary
 * passes complex values as opaque pointers, and because some legacy
 * paths predate the templated `aring` framework. Internally,
 * `M2::ARingCCC` (`aring-CCC.hpp`) is a thin C++ wrapper over this
 * same code; new engine code should reach for `ARingCCC` and let
 * this header carry the FFI surface.
 *
 * @see aring-CCC.hpp
 * @see aring-CC.hpp
 */

/* The interface is similar to mpfr:
   Every gmp_CC struct needs to be initialized with init or init_set.
   All rounding is MPFR_RNDN.
   Resulting values are the first argument
*/

#if !defined(SAFEC_EXPORTS)
//#include <engine-exports.h>
#include "interface/m2-types.h"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

  void mpfc_init(gmp_CCmutable result, long precision);
  void mpfc_clear(gmp_CCmutable result);
  void mpfc_init_set(gmp_CCmutable result, gmp_CCmutable a);
  void mpfc_set_si(gmp_CCmutable result, long re);
  void mpfc_set(gmp_CCmutable result, gmp_CCmutable a);
  int mpfc_is_zero(gmp_CCmutable a);
  int mpfc_is_equal(gmp_CCmutable a, gmp_CCmutable b);
  void mpfc_add(gmp_CCmutable result, gmp_CCmutable a, gmp_CCmutable b);
  void mpfc_neg(gmp_CCmutable result, gmp_CCmutable a);
  void mpfc_sub(gmp_CCmutable result, gmp_CCmutable a, gmp_CCmutable b);
  void mpfc_mul(gmp_CCmutable result, gmp_CCmutable a, gmp_CCmutable b);
  void mpfc_invert(gmp_CCmutable result, gmp_CCmutable v);

  void mpfc_sub_mult(gmp_CCmutable result, gmp_CCmutable a, gmp_CCmutable b);
  /* result -= a*b */

  void mpfc_div(gmp_CCmutable result, gmp_CCmutable a, gmp_CCmutable b);
  void mpfc_abs(gmp_RRmutable result, gmp_CCmutable a);
  void mpfc_sqrt(gmp_CCmutable result, gmp_CC a);
  void mpfc_conj(gmp_CCmutable result, gmp_CCmutable a);

#if defined(__cplusplus)
}
#endif

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
