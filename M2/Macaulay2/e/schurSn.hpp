// Copyright 2011 Michael E. Stillman

#ifndef _schurSn_hh_
#define _schurSn_hh_

/**
 * @file schurSn.hpp
 * @brief `SchurSnRing` --- symmetric-group representation ring built on `SchurRing2`.
 *
 * Declares `SchurSnRing`, a `SchurRing2` subclass whose elements
 * are characters of irreducible `S_n` representations (indexed
 * by partitions of `n`). The default constructor argument
 * `n = -1` selects the graded direct sum
 * `oplus_{n} R(S_n)` of all symmetric-group representation
 * rings; `n > 0` restricts to a single `R(S_n)`. The
 * `cast_to_SchurSnRing` overrides let engine code distinguish
 * this specialisation from a generic Schur ring without
 * resorting to `dynamic_cast`.
 *
 * Two products coexist: the inherited `SchurRing2::mult` is the
 * ordinary Schur-function product (used for symmetric-function
 * identities), while the added `tensor_mult` is the Kronecker
 * tensor product of characters --- the operation the
 * representation theorist actually calls "multiplication" in
 * `R(S_n)`. They agree under some interpretations but diverge
 * in general; both are exposed so callers can pick.
 *
 * @see schur2.hpp
 * @see schur-poly-heap.hpp
 */

#include "schur2.hpp"

class SchurSnRing : public SchurRing2
{
 public:
  SchurSnRing(const Ring *A, int n = -1);

  static SchurSnRing *create(const Ring *A, int n = -1);

  virtual const SchurSnRing *cast_to_SchurSnRing() const { return this; }
  virtual SchurSnRing *cast_to_SchurSnRing() { return this; }
  virtual ring_elem mult(const ring_elem f, const ring_elem g) const;
  ring_elem tensor_mult(const ring_elem f, const ring_elem g) const;
};

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
