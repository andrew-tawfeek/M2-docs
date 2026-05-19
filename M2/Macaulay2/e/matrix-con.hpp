#ifndef matrixcon_hpp_
#define matrixcon_hpp_

/**
 * @file matrix-con.hpp
 * @brief `MatrixConstructor` --- the mutable builder that produces an immutable `Matrix`.
 *
 * Declares `MatrixConstructor`, the only `friend` of the
 * private-constructor `Matrix` class. It holds the ring, target
 * and source `FreeModule*`s, and a working `VECTOR(vec)` of
 * columns, accumulating state with `set_column` and / or
 * `append_column` before handing back a finished immutable
 * `Matrix*` via `to_matrix()`. The `cols_frozen` flag tracks the
 * two construction modes: when the caller supplies the source
 * free module up front, modifications that would change the
 * column count are forbidden; when the source starts unset, the
 * builder grows it column-by-column and freezes it on completion.
 *
 * The split exists because `Matrix` is intentionally immutable
 * --- shared safely across threads and used as a memoisation key
 * --- but the engine still needs a place to accumulate column
 * data before degree-compatibility checks. `matrix-con.hpp`
 * provides exactly that scratch surface and nothing else.
 *
 * @see matrix.hpp
 * @see freemod.hpp
 */

#include <vector>
#include "ring.hpp"
#include <utility>

class MatrixGenerator;

/**
 * \ingroup matrices
 */
class MatrixConstructor
{
  const Ring *R;
  VECTOR(vec) entries;
  const FreeModule *rows;
  const FreeModule *cols;  // If cols is given at the beginning, this is used.
  // If this is immutable, no changes are allowed, other than to replace the
  // entire thing.

  bool cols_frozen;  // Once this is set, no more modifications to the 'cols'
                     // are allowed.  In particular, if the 'source' is set at
  // the beginning via the constructor, and that free module is
  // immutable, then no more changes are allowed.

  const_monomial deg;

  void compute_column_degree(int i);

 public:
  MatrixConstructor();
  MatrixConstructor(const FreeModule *target, int ncols);
  MatrixConstructor(const FreeModule *target,
                    const FreeModule *source,
                    const_monomial deg = nullptr);

  // The copy constructor just does the default thing: copy over all items.

  void append(vec v);
  void append(vec v, const_monomial deg);

  void set_entry(int r, int c, ring_elem a);
  void set_column(int c, vec v);

  void compute_column_degrees(); /* Takes into account the matrix degree */

  void set_column_degree(int i, const_monomial deg);

  void set_matrix_degree(const_monomial deg);

  Matrix *to_matrix();

  void debugDisplay() const;
};

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
