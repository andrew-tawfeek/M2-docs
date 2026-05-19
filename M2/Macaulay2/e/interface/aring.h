#ifndef _aring_h_
#  define _aring_h_

/**
 * @file interface/aring.h
 * @brief Engine-boundary C API for constructing `aring`-backed coefficient rings.
 *
 * Declares the `extern "C"` `rawARing*` factories the M2
 * interpreter calls (via the generated `d/engine.dd` bindings)
 * whenever the user asks for one of the `aring`-family
 * coefficient rings: prime fields (`rawARingZZp`), the native
 * and FLINT-backed Galois fields (`rawARingGaloisField`,
 * `rawARingGaloisFieldFlintBig`, `rawARingGaloisFieldFlintZech`,
 * `rawARingGaloisField1`), arbitrary-precision real and complex
 * fields, and the FLINT-accelerated `ZZ` / `QQ` variants. Each
 * function returns a `Ring*` wrapping the appropriate `aring`
 * instance or null on a domain error so the interpreter can
 * report it back to the user.
 *
 * The trailing `connected` annotations on individual entries
 * mark those that already have a matching `d/` binding;
 * adding a new `aring` requires declaring it here,
 * implementing in `aring.cpp`, and wiring the M2-side `.dd`
 * binding.
 *
 * @see aring.cpp
 * @see engine-includes.hpp
 */

#  include "engine-includes.hpp"

// TODO: fix this
#  if defined(__cplusplus)
class Ring;
class RingElement;
#  else
typedef struct Ring Ring;
typedef struct RingElement RingElement;
#  endif

/**
   ARing interface routines
 */

#  if defined(__cplusplus)
extern "C" {
#  endif

const Ring /* or null */ *rawARingZZp(unsigned long p); /* connected */
/* Expects a prime number p in range 2 <= p <= 32749 */

const Ring /* or null */ *rawARingGaloisField1(
    const RingElement *prim); /* connected */
/* same interface as rawGaloisField, but uses different internal code */

const Ring /* or null */ *rawARingGaloisFieldFlintBig(
    const RingElement *prim); /* connected */
/* same interface as rawGaloisField, but uses Flint GF code designed for
   wordsize p, but too big for lookup tables */

const Ring /* or null */ *rawARingGaloisFieldFlintZech(
    const RingElement *prim); /* connected */
/* same interface as rawGaloisField, but uses Flint GF code designed for
   wordsize p, and uses lookup tables */

const Ring /* or null */ *rawARingGaloisField(int p, int n); /* connected */
/* creates a ring GF(p^n).  Constraints on p, n? */
/* returns null if the values p,n are too large  */

const Ring /* or null */ *rawARingGaloisFieldFromQuotient(
    const RingElement *prim); /* connected */
/* same interface as rawGaloisField, but uses Givaro */

M2_arrayintOrNull rawARingGFPolynomial(const Ring *R);
/* given an ARingGF, return the coefficient array of the quotient polynomial.
   So, if R = kk[a]/(f(a)), where kk = ZZ/p, then the (integer) coefficients
   {f0, f1, f2, ..., f_(degree f)} is returned.
 */

M2_arrayintOrNull rawARingGFCoefficients(const RingElement *f);
/* f can be written as a polynomial in the generator a, e.g.
   f = f0 + a*f1 + ... + a^(d-1) * f_(d-1), where d = deg of the
   ring over ZZ/p.  This function returns {f0, f1, ..., f_(d-1)},
   where each entry is an integer */

const Ring /* or null */ *rawARingTower1(const Ring *K, M2_ArrayString names);

const Ring /* or null */ *rawARingTower2(const Ring *R1,
                                         M2_ArrayString new_names);

const Ring /* or null */ *rawARingTower3(const Ring *R1,
                                         engine_RawRingElementArray eqns);

const RingElement *rawMultiplicativeGenerator(const Ring *R);
/* given an ARingGF, return the  the generator of the multiplicative group.
 */

/**
   ARing Flint routines
 */

const Ring /* or null */ *rawARingZZFlint(); /* connected */

const Ring /* or null */ *rawARingQQFlint(); /* connected */

const Ring /* or null */ *rawARingZZpFlint(unsigned long p); /* connected */
/* Expects a prime number p in range 2 <= p <= 2^64-1 */

#  if defined(__cplusplus)
}
#  endif

#endif /* _aring_h_ */

// Local Variables:
// indent-tabs-mode: nil
// End:
