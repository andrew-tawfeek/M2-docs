#ifndef _factory_h_
#  define _factory_h_

/**
 * @file interface/factory.h
 * @brief Engine-boundary C API for polynomial GCD, factorisation, and root finding.
 *
 * Declares the `extern "C"` entry points the M2 interpreter
 * routes to Singular's Factory library (and MPSolve for
 * numerical complex roots): `rawGCDRingElement` and
 * `rawExtendedGCDRingElement` for univariate / multivariate GCD
 * with optional algebraic-extension minimal polynomial,
 * `rawPseudoRemainder` for division with leading-coefficient
 * scaling, `rawFactor` / `rawFactor2` for factorisation of
 * `RingElement`s (the latter accepting a minimal polynomial so
 * factorisation can run over an algebraic extension),
 * `rawIdealReorder` / `rawCharSeries` for characteristic-set
 * preprocessing of polynomial ideals, and `rawRoots` for
 * arbitrary-precision numerical roots of univariate polynomials
 * via MPSolve.
 *
 * The dual `class` / `typedef struct` declarations of `Matrix`
 * and `RingElement` keep the header usable from both C++ and
 * the generated-C side of the `.dd` pipeline. If Factory or
 * MPSolve is absent at build time, the corresponding entries
 * return null and the M2 layer reports "feature not available."
 *
 * @see factory.cpp
 * @see flint.h
 * @see engine-includes.hpp
 */

#  include "engine-includes.hpp"

// TODO: fix this
#  if defined(__cplusplus)
class Matrix;
class RingElement;
#  else
typedef struct Matrix Matrix;
typedef struct RingElement RingElement;
#  endif

/**
   Polynomial GCD and factorization interface routines via Factory and MPSolve
 */

#  if defined(__cplusplus)
extern "C" {
#  endif

const RingElement *rawGCDRingElement(const RingElement *f,
                                     const RingElement *g,
                                     const RingElement *mipo,
                                     M2_bool inExtension);

const RingElement *rawExtendedGCDRingElement(const RingElement *f,
                                             const RingElement *g,
                                             const RingElement **A,
                                             const RingElement **B);

const RingElement *rawPseudoRemainder(const RingElement *f,
                                      const RingElement *g);

void rawFactor(const RingElement *f,
               engine_RawRingElementArrayOrNull *result_factors,
               M2_arrayintOrNull *result_powers);

void rawFactor2(const RingElement *f,
                const RingElement *minpoly,
                engine_RawRingElementArrayOrNull *result_factors,
                M2_arrayintOrNull *result_powers);

M2_arrayintOrNull rawIdealReorder(const Matrix *M);

engine_RawMatrixArrayOrNull rawCharSeries(const Matrix *M);

/* uses MPSolve */
engine_RawRingElementArrayOrNull rawRoots(const RingElement *g,
                                          long prec,
                                          int unique);

#  if defined(__cplusplus)
}
#  endif

#endif /* _factory_h_ */

// Local Variables:
// indent-tabs-mode: nil
// End:
