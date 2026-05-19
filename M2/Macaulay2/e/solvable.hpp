// Copyright 2003 Michael E. Stillman

#ifndef _solvable_hh_
#define _solvable_hh_

/**
 * @file solvable.hpp
 * @brief `SolvableAlgebra` --- PBW algebra where `x_j x_i = x_i x_j + q_{ij}` for fixed commutators.
 *
 * Declares `SolvableAlgebra`, the `PolyRing` subclass for
 * polynomial-shaped algebras whose generators do not commute
 * but satisfy a PBW-style straightening relation
 * `x_j x_i = x_i x_j + q_{ij}` for `i < j`, with each
 * commutator polynomial `q_{ij}` supplied as the `(i, j)` entry
 * of a constructor-provided `Matrix Q_`. The framework covers
 * Weyl algebras, quantum planes, universal enveloping algebras
 * `U(g)`, and the trivial zero-`Q` case where the algebra
 * degenerates to an ordinary polynomial ring. Multiplication
 * walks both factors and substitutes `x_j x_i -> x_i x_j +
 * q_{ij}` whenever it encounters an out-of-order product,
 * recursing on `q_{ij}`.
 *
 * Termination requires every `q_{ij}` to lie strictly below
 * `x_i x_j` in some well-founded ordering --- the PBW
 * condition. The constructor checks this lazily and trusts the
 * caller to supply a `Q` that defines an associative algebra.
 *
 * @see poly.hpp
 * @see weylalg.hpp
 * @see skewpoly.hpp
 */

#include "poly.hpp"

///// Ring Hierarchy ///////////////////////////////////

class SolvableAlgebra : public PolyRing
{
  const Matrix *Q_;

  static SolvableAlgebra *create(const Ring *K,
                                 const Monoid *M,
                                 const Matrix *Q);

 protected:
  bool initialize_solvable(const Matrix *Q);
  SolvableAlgebra() {}
  virtual ~SolvableAlgebra();

 public:
  static SolvableAlgebra *create(const PolynomialRing *R, const Matrix *Q);

  virtual bool is_commutative_ring() const { return false; }
  virtual bool is_solvable_algebra() const { return true; }
  virtual const SolvableAlgebra *cast_to_SolvableAlgebra() const
  {
    return this;
  }
  virtual SolvableAlgebra *cast_to_SolvableAlgebra() { return this; }
  virtual ring_elem power(const ring_elem f, mpz_srcptr n) const;
  virtual ring_elem power(const ring_elem f, int n) const;

 public:
  virtual ring_elem mult_by_term(const ring_elem f,
                                 const ring_elem c,
                                 const int *m) const;
};
#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
