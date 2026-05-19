// Copyright 2004 Michael E. Stillman

#ifndef _comp_hpp_
#define _comp_hpp_

/**
 * @file comp.hpp
 * @brief Abstract `Computation` base class --- stop-condition machinery for incremental engine work.
 *
 * `Computation` is the pure-virtual root of every long-running
 * engine task (Groebner basis, resolution, Hilbert series, ...). It
 * carries a `ComputationStatusCode` (`COMP_DONE`,
 * `COMP_INTERRUPTED`, `COMP_DEGREE_LIMIT_REACHED`, ...) and a
 * `StopConditions` struct that bundles the user-facing limit
 * vocabulary: `degree_limit`, `basis_element_limit`, `syzygy_limit`,
 * `pair_limit`, `codim_limit`, `subring_limit`, `just_min_gens`,
 * `length_limit`, and the `always_stop` debug knob. M2 statements
 * like `gb(I, DegreeLimit => 5)` map to constructing a concrete
 * subclass, calling `set_stop_conditions`, then `start_computation`.
 *
 * The incremental design is what makes the engine usable on
 * hour-or-day computations: users can interrupt cleanly (status
 * flips to `COMP_INTERRUPTED`), inspect the partial state, raise
 * a limit, and call `start_computation` again to resume where the
 * previous run stopped. The same pattern shows up in every
 * `comp-*` subclass.
 *
 * @see comp-gb.hpp
 * @see comp-res.hpp
 * @see interface/computation.h
 */

#include "interface/computation.h"
#include "hash.hpp"

class GBComputation;
class ResolutionComputation;

class buffer;

/**
    @ingroup computations
*/

class Computation : public MutableEngineObject
{
 private:
  enum ComputationStatusCode computation_status;

 protected:
  StopConditions stop_;

  Computation();

  enum ComputationStatusCode set_status(enum ComputationStatusCode);

  virtual bool stop_conditions_ok() = 0;
  // If the stop conditions in stop_ are inappropriate,
  // return false, and use ERROR(...) to provide an error message.

  virtual ~Computation();

 public:
  Computation /* or null */ *set_stop_conditions(M2_bool always_stop,
                                                 M2_arrayint degree_limit,
                                                 int basis_element_limit,
                                                 int syzygy_limit,
                                                 int pair_limit,
                                                 int codim_limit,
                                                 int subring_limit,
                                                 M2_bool just_min_gens,
                                                 M2_arrayint length_limit);
  // returns NULL if there is a general problem with one of the stop
  // conditions.

  enum ComputationStatusCode status() const { return computation_status; }
  virtual int complete_thru_degree() const = 0;
  // This is computation specific information.  However, for homogeneous
  // GB's, the GB coincides with the actual GB in degrees <= the returned value.
  // For resolutions of homogeneous modules, the resolution
  // coincides with the actual one in (slanted) degrees <= the returned value.

  virtual void start_computation() = 0;
  // Do the computation as specified by the stop conditions.
  // This routine should set the status of the computation.

  virtual GBComputation *cast_to_GBComputation() { return nullptr; }
  virtual ResolutionComputation *cast_to_ResolutionComputation() { return nullptr; }
  virtual void text_out(buffer &o) const;

  virtual void show() const;  // debug display of some computations
};

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
