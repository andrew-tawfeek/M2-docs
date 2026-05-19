/* Copyright 2010, Michael E. Stillman */

#ifndef _interreduce_h_
#define _interreduce_h_

/**
 * @file interreduce.hpp
 * @brief `Interreducer` --- inter-reduction of a list of `gbvector*` elements over a `GBRing`.
 *
 * Declares `Interreducer`, the helper that brings a collection of
 * `gbvector*` polynomials into a head-reduced state: for every
 * pair `(f_i, f_j)` whose leading terms divide one another, the
 * smaller-pivot cancellation is applied until no further leading-
 * term reduction is possible. The class stores its working
 * `GBRing*`, `FreeModule*`, and the live `VECTOR(gbvector*)` it
 * is updating in place. Two atomic reduction primitives drive the
 * loop: `cancelLT(f, g)` cancels the leading term of `f` using
 * `g` and returns the number of reductions performed, and
 * `reduceTail(f, g)` keeps going until no term of `f` is
 * divisible by `lt(g)`. `showElem` / `show` are debug printers
 * that dump the first `nterms` of each element.
 *
 * Inter-reduction is a stable building block reused across the
 * engine: the default GB algorithm in `gb-default.hpp` calls into
 * it at finalisation, `reducedgb.hpp` uses it as part of
 * canonicalising the reduced GB, and `forceGB` runs it to drag a
 * user-supplied basis into shape.
 *
 * @see gbring.hpp
 * @see reducedgb.hpp
 * @see gb-default.hpp
 */

#include "gbring.hpp"

class Interreducer
{
  GBRing *R;
  FreeModule *F;
  VECTOR(gbvector *) G;

 public:
  Interreducer(GBRing *R, FreeModule *F, VECTOR(gbvector *) & elems0);

  void showElem(int i, int nterms);
  void show(int nterms);

  int cancelLT(gbvector *&f, const gbvector *g);
  // reduces f by g, until lt(f) is not divisible by lt(g).
  // returns #reductions performed.

  int reduceTail(gbvector *&f, const gbvector *g);
  // reduces f by g, until no term of f is divisible by lt(g).
  // returns #reductions performed.

  bool reduceLT(gbvector *&f, int i);
  // reduces f w.r.t G \ i.

  bool reduceTail(gbvector *&f, int i);
  // reduces f w.r.t G \ i.
};

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e"
// indent-tabs-mode: nil
// End:
