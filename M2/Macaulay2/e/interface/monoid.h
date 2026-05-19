#ifndef _monoid_h_
#  define _monoid_h_

/**
 * @file interface/monoid.h
 * @brief Engine-boundary C API for constructing and inspecting `Monoid` objects.
 *
 * Declares the `extern "C"` entry points the M2 interpreter
 * calls to build the monoid of monomials underneath every
 * polynomial ring. `rawTrivialMonoid()` returns the cached
 * single-instance trivial monoid; `rawMonoid(mo, deg_ring,
 * names, degrees, heftvec)` is the workhorse constructor that
 * takes a `MonomialOrdering`, a degree-monoid pointer
 * (`deg_ring`), the variable names, a flat degree vector, and a
 * heft vector, and returns a fresh `Monoid*` or null if the
 * inputs are inconsistent. `rawMonoidNumberOfBlocks`,
 * `rawMonoidHash`, `rawMonoidToString`, and the helper
 * `to_degree_vector` cover the per-monoid read accessors the
 * interpreter needs after construction.
 *
 * The `deg_t = int32_t` and `const_monomial` typedefs declared
 * here are the engine-wide format for degree slots and packed
 * monomials passed back through this surface. The recursive
 * `deg_ring` argument is what makes multi-degrees expressible:
 * the degrees of a `Monoid` live in another `Monoid`, with the
 * trivial monoid as the base case --- M2 builds the chain by
 * calling `rawMonoid` repeatedly.
 *
 * @see monoid.cpp
 * @see monomial-ordering.h
 * @see engine-includes.hpp
 */

#  include "engine-includes.hpp"

// TODO: make this unnecessary
#  if defined(__cplusplus)
class Ring;
class Monoid;
class MonomialOrdering;
#  else
typedef struct Ring Ring;
typedef struct Monoid Monoid;
typedef struct MonomialOrdering MonomialOrdering;
#  endif

/**
   Monoid interface routines
 */

#  if defined(__cplusplus)
extern "C" {
#  endif

/* integer types to be used for degrees and exponents */
typedef int32_t deg_t;
typedef const deg_t *const_monomial;

/* rawMonoid(): always returns the same object */
const Monoid *rawTrivialMonoid();

/* rawMonoid(mo, deg_ring, names, degrees, heftvec) */
const Monoid * /* or Null */ rawMonoid(const MonomialOrdering *mo,
                                       const Ring *deg_ring,
                                       M2_ArrayString names,
                                       M2_arrayint degrees,
                                       M2_arrayint heftvec);
// TODO: this is wrong and needs to be updated
/* This function will return NULL if the monomial order cannot be handled
   currently, if the first components for each degree are not all
   positive, or under various other "internal" error conditions */

int rawMonoidNumberOfBlocks(const Monoid *M);

/* connected to the interpreter in d/basic.d */
unsigned int rawMonoidHash(const Monoid *M);
/* Assigned sequentially */

/* connected to the interpreter in d/actors4.d */
M2_string rawMonoidToString(const Monoid *M);
/* For debugging purposes */

/* Service routine for converting degree monomials to int arrays.
   To be used only by other interface routines. */
M2_arrayint to_degree_vector(const Monoid *M, const_monomial d);

#  if defined(__cplusplus)
}
#  endif

#endif /* _monoid_h_ */

// Local Variables:
// indent-tabs-mode: nil
// End:
