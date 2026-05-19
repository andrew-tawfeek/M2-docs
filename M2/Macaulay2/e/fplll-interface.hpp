/**
 * @file fplll-interface.hpp
 * @brief Engine-side wrapper around the external [fplll](https://github.com/fplll/fplll) lattice-reduction library.
 *
 * Declares a single entry point, `fp_LLL(M, U, strategy)`: `M`
 * holds the lattice basis (rows are the basis vectors) and is
 * reduced in place; `U`, if non-null, receives the unimodular
 * transformation expressing the new basis in terms of the original;
 * `strategy` picks an algorithm flavour (`0` = LLL, `1` = BKZ,
 * etc.). Returns `true` on success.
 *
 * The implementation in `fplll-interface.cpp` is gated behind
 * `#ifdef HAVE_FPLLL`; when fplll is not installed at configure
 * time the function returns `false` so callers do not need their
 * own `#ifdef`. M2's `LLL` user-facing routine dispatches between
 * this path, the NTL-backed `LLLglue.cpp`, and the slower pure-M2
 * fallback in `LLL.hpp`; this wrapper is typically the fastest
 * when available.
 *
 * @see mat.hpp
 * @see LLL.hpp
 */

class MutableMatrix;

bool fp_LLL(MutableMatrix *M, MutableMatrix *U, int strategy);

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e  "
// indent-tabs-mode: nil
// End:
