// Copyright 2010 Michael E. Stillman.

/**
 * @file finalize.hpp
 * @brief `intern_*` helpers that register long-lived engine objects with bdwgc finalisers.
 *
 * Declares `intern_polyring`, `intern_monideal`,
 * `internMutableMatrix`, `intern_GB`, `intern_res`, and
 * `intern_SchreyerOrder`. Each one takes a freshly constructed
 * engine object that will outlive its caller (typically because it
 * is being returned to the interpreter) and registers it with bdwgc
 * so the corresponding C++ destructor --- plus any externally-held
 * FLINT contexts, NTL state, or OS handles --- runs when the
 * collector decides the object is unreachable. Without this step
 * bdwgc would simply reclaim the bytes, and the externally-tracked
 * resources would leak.
 *
 * Objects owned by stack-based holders that delete explicitly do
 * not need to be interned; the header comment is precise about
 * "if G will not be freed by its owner." `internMutableMatrix` is
 * the one helper that returns a pointer rather than just
 * registering, supporting an ownership-transfer pattern in
 * snapshotting code. Heavy callers are the interpreter glue in
 * `d/engine.dd` and the top-level engine factories.
 *
 * @see newdelete.hpp
 */

class PolynomialRing;
class MonomialIdeal;
class MutableMatrix;
class GBComputation;
class ResolutionComputation;
class SchreyerOrder;

// These functions should be called if G will not be freed by its owner
void intern_polyring(const PolynomialRing *G);
void intern_monideal(MonomialIdeal *G);
MutableMatrix *internMutableMatrix(MutableMatrix *G);
void intern_GB(GBComputation *G);
void intern_res(ResolutionComputation *G);
void intern_SchreyerOrder(SchreyerOrder *G);
