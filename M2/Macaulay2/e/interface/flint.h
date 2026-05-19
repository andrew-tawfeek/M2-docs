#ifndef _flint_h_
#  define _flint_h_

/**
 * @file interface/flint.h
 * @brief Engine-boundary C API exposing FLINT's integer primality and factorisation services.
 *
 * Declares the three `extern "C"` entry points the M2
 * interpreter routes to FLINT for big-integer number theory:
 * `rawZZisPrime` (exact primality via `fmpz_is_prime`),
 * `rawZZisProbablePrime` (Miller-Rabin, used when exact primality
 * would be prohibitively slow on multi-thousand-digit inputs),
 * and `rawZZfactor` (full integer factorisation, returned as a
 * `gmp_arrayZZ` of alternating primes and multiplicities).
 * Inputs arrive as `gmp_ZZ` and the implementation handles the
 * GMP-to-FLINT conversion internally so the interpreter never
 * sees `fmpz_t`.
 *
 * FLINT is consumed pervasively *inside* the engine (see the
 * `aring-*-flint`, `dmat-*-flint`, and `polyroots.cpp` files),
 * but this header is the only place where FLINT-backed services
 * are exposed directly to the interpreter; everything else stays
 * private to its owning class.
 *
 * @see flint.cpp
 * @see factory.h
 * @see engine-includes.hpp
 */

#  include "engine-includes.hpp"

/**
   Integer primality and factorization interface routines via Flint
 */

#  if defined(__cplusplus)
extern "C" {
#  endif

M2_bool rawZZisPrime(gmp_ZZ a);

M2_bool rawZZisProbablePrime(gmp_ZZ a);

gmp_arrayZZ rawZZfactor(gmp_ZZ a);

#  if defined(__cplusplus)
}
#  endif

#endif /* _flint_h_ */

// Local Variables:
// indent-tabs-mode: nil
// End:
