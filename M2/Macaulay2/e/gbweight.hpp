// Copyright 2004.  Michael E. Stillman

#ifndef _gbweight_hpp_
#define _gbweight_hpp_

/**
 * @file gbweight.hpp
 * @brief `GBWeight` --- packed-weight evaluator that drives S-pair selection.
 *
 * Declares `GBWeight`, the helper that GB code consults to assign
 * a heuristic weight to a `gbvector` or to a single
 * `(monomial, component)` pair. Construction takes a weight
 * vector `wts_`, the ambient `FreeModule` and `GBRing`, and a
 * `use_component_degrees_` toggle that, when true, adds the
 * component's stored degree to the result. Callers use the
 * returned weight as a sort key on S-pairs (degree-first or
 * sugar-first orderings, custom weightings asked for at the M2
 * level, ...) --- `GBWeight` itself does not choose a policy, it
 * computes whichever weighting was configured.
 *
 * The implementation is intentionally minimal because the
 * functions are called millions of times per GB run: a packed
 * dot product of the exponent vector with `wts_`, plus a
 * constant-time per-component lookup when component degrees are
 * enabled. `gbvector_weight` also reports the maximum component
 * degree it encountered along the way so the S-pair queue can
 * use it directly.
 *
 * @see gbring.hpp
 * @see spair.hpp
 * @see gb-default.hpp
 */

#include "engine-includes.hpp"  // for M2_arrayint

#include "ExponentVector.hpp"   // for const_exponents
#include "monoid.hpp"           // for const_monomial
#include "newdelete.hpp"        // for our_new_delete

class FreeModule;
class GBRing;
class gbvector;

class GBWeight : public our_new_delete
// A class to compute the "heuristic weight" of elements
// Mainly for use with Groebner basis computation
{
  M2_arrayint wts_;
  bool use_component_degrees_;
  const FreeModule *F_;

  int nvars_;
  GBRing *R_;

  size_t exp_size;  // length in bytes of temporary exponent vectors
  long *Fdegs_;

 public:
  GBWeight(const FreeModule *F, M2_arrayint wts);

  ~GBWeight() {}
  M2_arrayint get_weights() const { return wts_; }
  // All of these routines expect components in the range 1..N,
  // or, if the component is 0, then it is in the ring, not in the
  // free module F_

  int exponents_weight(const_exponents e, int comp) const;

  int gbvector_term_weight(const gbvector *f) const;

  int gbvector_weight(const gbvector *f) const;

  int gbvector_weight(const gbvector *f, int &lead_term_weight) const;

  int monomial_weight(const_monomial m, int comp) const;
};

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
