// Copyright 2016  Michael E. Stillman

#ifndef _res_moninfo_hpp_
#define _res_moninfo_hpp_

/**
 * @file schreyer-resolution/res-moninfo.hpp
 * @brief `ResMonoid` dispatcher --- single switch between `ResMonoidDense` and `ResMonoidSparse`.
 *
 * Pulls in both monomial-layout implementations the F4
 * resolution uses and aliases `ResMonoid` to the active one
 * (`ResMonoidDense` in production, `ResMonoidSparse` kept
 * available behind a commented-out `using`). The dense layout
 * stores each monomial as a length-`nvars` exponent vector and
 * wins for dense monomials or small variable counts; the sparse
 * layout stores `(variable, exponent)` pairs and wins for
 * low-support monomials in many-variable rings. Both expose the
 * same `monomial_size`, `compare`, `multiply`, and
 * `to_expvector` / `from_expvector` surface, so switching only
 * requires editing this header.
 *
 * The rest of the schreyer-resolution code references
 * `ResMonoid` through this typedef, which keeps the
 * implementation choice pinned to a single file and lets
 * benchmark-driven swaps stay zero-touch at call sites. Future
 * builds may make the choice configure-time rather than
 * compile-time, but for now the comment is the only mechanism.
 *
 * @see res-moninfo-dense.hpp
 * @see res-moninfo-sparse.hpp
 * @see res-poly-ring.hpp
 * @see res-schreyer-frame.hpp
 */

#include "schreyer-resolution/res-moninfo-dense.hpp"
#include "schreyer-resolution/res-moninfo-sparse.hpp"

using ResMonoid = ResMonoidDense;
// using ResMonoid = ResMonoidSparse;

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
