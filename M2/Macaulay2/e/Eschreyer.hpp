// Copyright 1999  Michael E. Stillman
#ifndef _Eschreyer_hpp_
#define _Eschreyer_hpp_

/**
 * @file Eschreyer.hpp
 * @brief Older Schreyer-style kernel computation, predecessor of `schreyer-resolution/`.
 *
 * Declares `GBKernelComputation` --- a `Computation` subclass that,
 * given a matrix `f : F -> G` whose columns are a Groebner basis of
 * `im(f)`, produces the kernel `K` of `f` as a set of generators which
 * are themselves a Groebner basis under the Schreyer order on `F`
 * induced by `(f, lt(f_1), ..., lt(f_n))`. That structural guarantee
 * is what makes the routine useful for iterated free resolutions: the
 * kernel is in the right form to feed straight into the next step.
 *
 * Also declares the lightweight `GBMatrix` --- a matrix whose columns
 * are owned `gbvector*` rather than opaque `vec` values, so the inner
 * loop sidesteps a `Matrix -> gbvector` conversion every iteration.
 * The newer `schreyer-resolution/` implementation reduces a whole
 * degree at once via F4 and is faster on most inputs; this older
 * single-row reduction is retained for regression testing and the rare
 * small-example case where it still wins.
 *
 * @see gbring.hpp
 * @see schorder.hpp
 * @see comp.hpp
 */

#include "polyring.hpp"
#include "gbring.hpp"
#include "schorder.hpp"
#include "matrix.hpp"
#include "monoid.hpp"
#include "comp.hpp"

struct GBMatrix : public our_new_delete
{
  const FreeModule *F;  // target
  gc_vector<gbvector*> elems;

  GBMatrix(const Matrix *m);
  GBMatrix(const FreeModule *F);
  void append(gbvector *f);  // grabs f
  const FreeModule *get_free_module() const { return F; }
  Matrix *to_matrix();
};

class GBKernelComputation : public Computation
{
  // these three were virtual in class Computation
  bool stop_conditions_ok() { return true; }
  int complete_thru_degree() const { return 0; }
  void start_computation() {}
  const PolynomialRing *R;
  const Ring *K;
  GBRing *GR;
  const Monoid *M;
  const SchreyerOrder *SF;  // order for F.
  const SchreyerOrder *SG;  // order for G.
  const FreeModule *F;      // This is where the action is...
  const FreeModule *G;      // This is where the resulting syzygies live.
  // This MUST be a Schreyer free module compatible with the input!

  gc_vector<MonomialIdeal*> mi;   // Used in reduction.
  gc_vector<gbvector*> gb;        // This is the "stripped" GB.
  gc_vector<gbvector*> syzygies;  // This is basically the result.

  // byte sizes for allocating temp exp vectors and monomials on the stack
  size_t exp_size;
  size_t monom_size;

  int n_ones;
  int n_unique;
  int n_others;
  int total_reduce_count;

  void new_pairs(int i);
  void strip_gb(const gc_vector<gbvector*> &m);
  void strip_gb(const GBMatrix *m);

  gbvector *make_syz_term(ring_elem c, const_monomial monom, int comp) const;
  // This routine grabs 'c', and 'monom' should be the total monomial.

  bool find_ring_divisor(const_exponents exp, const gbvector *&result);
  int find_divisor(const MonomialIdeal *mi, const_exponents exp, int &result);
  // Returns the index of the least element in the monomial order which divides.

  void wipe_unneeded_terms(gbvector *&f);
  // removes every term of f which is not a lead term of some element of gb.

  gbvector *s_pair(gbvector *syz);
  void reduce(gbvector *&g,
              gbvector *&gsyz);  // Reduces g to zero.  gsyz is real result.
  void geo_reduce(gbvector *&g,
                  gbvector *&gsyz);  // Reduces g to zero.  gsyz is real result.
 public:
  GBKernelComputation(const GBMatrix *m);

  virtual ~GBKernelComputation();

  int calc();

  GBMatrix *get_syzygies();

 public:
  GBKernelComputation *cast_to_GBKernelComputation() { return this; }
  const GBKernelComputation *cast_to_GBKernelComputation() const
  {
    return this;
  }
};

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
