// Copyright 2004 Michael E. Stillman.

#ifndef _comp_res_hpp_
#define _comp_res_hpp_

/**
 * @file comp-res.hpp
 * @brief `ResolutionComputation` --- abstract base for every free-resolution algorithm in the engine.
 *
 * `ResolutionComputation` is the resolution-side mirror of
 * `GBComputation`: a `Computation` subclass that adds the
 * resolution-specific virtuals `get_free(level)`,
 * `get_matrix(level)` (the differential `F_level -> F_{level - 1}`),
 * `get_betti(type)` (Betti table with `type` selecting `minimal`,
 * `total`, `graded`, ...), and `complete_thru_degree`. Concrete
 * subclasses span the engine's resolution algorithms ---
 * `res_comp`, `res2_comp`, `ResolutionComputationA2`,
 * `EschreyerComputation`, `F4ResComputation` in
 * `schreyer-resolution/`, and `NCResComputation` --- each keeping
 * its own internal layout.
 *
 * The factory `ResolutionComputation::choose_res(...)` selects an
 * implementation from M2's `Strategy =>` option; older strategies
 * remain alive primarily for regression and the occasional small
 * input on which they still win. Per-degree state lives on the heap
 * so a typical `Strategy => N, DegreeLimit => k` workflow can
 * inspect Betti numbers, decide whether to continue, and resume by
 * relaxing the limit and calling `start_computation` again.
 *
 * @see comp.hpp
 * @see comp-gb.hpp
 * @see Eschreyer.hpp
 * @see betti.hpp
 */

#include "comp.hpp"
class buffer;

/**
    @ingroup res

    @brief Base class for free resolution computation classes.
*/

class ResolutionComputation : public Computation
// This is the base type for all resolution computations
{
 protected:
  ResolutionComputation();

  virtual bool stop_conditions_ok() = 0;
  // If the stop conditions in stop_ are inappropriate,
  // return false, and use ERROR(...) to provide an error message.

 public:
  // These three routines should be moved to a utility class
  static void betti_init(int lo, int hi, int len, int *&bettis);
  static M2_arrayint betti_make(int lo, int hi, int len, int *bettis);
  static void betti_display(buffer &o, M2_arrayint a);

 public:
  virtual ResolutionComputation *cast_to_ResolutionComputation()
  {
    return this;
  }

  virtual ~ResolutionComputation();

  static ResolutionComputation *choose_res(const Matrix *m,
                                           M2_bool resolve_cokernel,
                                           int max_level,
                                           M2_bool use_max_slanted_degree,
                                           int max_slanted_degree,
                                           int algorithm,
                                           int strategy,
                                           int numThreads,
                                           M2_bool parallelizeByDegree);
  // Values for algorithm and strategy are documented in engine.h

  virtual void start_computation() = 0;

  //  virtual ComputationStatusCode compute(const StopConditions &stop_, long
  //  &complete_thru_this_degree);
  virtual int complete_thru_degree() const = 0;
  // The computation is complete up through this slanted degree.

  ////////////////////////////////
  // Results of the computation //
  ////////////////////////////////
  virtual const Matrix /* or null */ *get_matrix(int level) = 0;

  virtual MutableMatrix /* or null */ *get_matrix(int level, int degree);
  // the default version gives an error that it isn't defined

  virtual const FreeModule /* or null */ *get_free(int level) = 0;

  virtual M2_arrayint get_betti(int type) const = 0;
  // type is documented under rawResolutionBetti, in engine.h

  //////////////////////////////////////
  // Statistics and spair information //
  //////////////////////////////////////

  virtual void text_out(buffer &o) const = 0;
  // This displays statistical information, and depends on the
  // M2_gbTrace value.
};

void intern_res(ResolutionComputation *G);

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
