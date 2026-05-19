#ifndef _cone_h_
#  define _cone_h_

/**
 * @file interface/cone.h
 * @brief Engine-boundary C API for rational polyhedral cone operations.
 *
 * Declares the two `extern "C"` entry points the M2 interpreter
 * uses for cone work: `rawFourierMotzkin(C)`, which dualises a
 * cone between facet-inequality and ray-generator representations
 * via Fourier-Motzkin elimination, and `rawHilbertBasis(C)`,
 * which returns the Hilbert basis (minimal generators of the
 * semigroup of lattice points in the cone) via Normaliz. Cones
 * are passed in and out as `Matrix*` whose columns are the
 * relevant rays or facets, so only `Matrix` needs to be forward-
 * declared at the C boundary.
 *
 * Consumed by the `Polyhedra`, `Normaliz`, and toric-geometry M2
 * packages. When Normaliz is unavailable on the build machine
 * the functions return null and the M2 layer surfaces a
 * "feature not available" error.
 *
 * @see cone.cpp
 * @see engine-includes.hpp
 */

#  include "engine-includes.hpp"

// TODO: fix this
#  if defined(__cplusplus)
class Matrix;
#  else
typedef struct Matrix Matrix;
#  endif

/**
   Cone interface routines
 */

#  if defined(__cplusplus)
extern "C" {
#  endif

/**************************************************/
/**** Cone routines (via Normaliz) ****************/
/**************************************************/

const Matrix /* or null */ *rawFourierMotzkin(const Matrix *C);

const Matrix /* or null */ *rawHilbertBasis(const Matrix *C);

#  if defined(__cplusplus)
}
#  endif

#endif /* _cone_h_ */
