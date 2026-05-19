#ifndef __eigen_hpp_
#define __eigen_hpp_

/**
 * @file eigen.hpp
 * @brief `EigenM2` namespace --- SVD / eigenvalues / eigenvectors over `DMat<R>` for `RR`, `CC`, `RRR`, `CCC`.
 *
 * Declares aliases `LMatrixRR`, `LMatrixCC`, `LMatrixRRR`,
 * `LMatrixCCC` over the four `DMat<R>` instantiations the engine
 * cares about for numerical linear algebra, then a namespace
 * `EigenM2` of free-function wrappers --- `SVD`,
 * `SVD_divide_conquer`, `eigenvalues`, `eigenvectors`, ... ---
 * with LAPACK-style signatures that operate on those `DMat`
 * pointers. Each routine returns `bool` so the engine can surface
 * convergence failures to the M2 user.
 *
 * The actual numerical work is split: hardware-precision `RR` /
 * `CC` matrices go through LAPACK (`lapack.hpp`); MPFR-precision
 * `RRR` / `CCC` matrices route through the Eigen3 template library,
 * which supports arbitrary scalar types at the cost of speed.
 * Interval rings `RRi` / `CCi` are not yet plumbed through here.
 * NAG (`NAG.hpp`) reaches into these routines for Jacobian
 * eigenvalue queries during continuation, and the M2-side `SVD`,
 * `eigenvalues`, `eigenvectors`, `LUdecomposition` built-ins
 * forward through them.
 *
 * @see lapack.hpp
 * @see dmat.hpp
 * @see NAG.hpp
 */

#include "dmat.hpp"
#include "aring-RR.hpp"
#include "aring-CC.hpp"
#include "aring-RRR.hpp"
#include "aring-CCC.hpp"
using LMatrixRR = DMat<M2::ARingRR>;
using LMatrixCC = DMat<M2::ARingCC>;
using LMatrixRRR = DMat<M2::ARingRRR>;
using LMatrixCCC = DMat<M2::ARingCCC>;

namespace EigenM2 {
  bool SVD(const LMatrixRR *A,
    LMatrixRR *Sigma,
    LMatrixRR *U,
    LMatrixRR *VT);
  bool SVD(const LMatrixCC *A,
    LMatrixRR *Sigma,
    LMatrixCC *U,
    LMatrixCC *VT);
  bool SVD_divide_conquer(const LMatrixRR *A,
    LMatrixRR *Sigma,
    LMatrixRR *U,
    LMatrixRR *VT);
  bool SVD_divide_conquer(const LMatrixCC *A,
    LMatrixRR *Sigma,
    LMatrixCC *U,
    LMatrixCC *VT);
  bool eigenvalues(const LMatrixRR *A, LMatrixCC *eigenvals);
  bool eigenvalues(const LMatrixCC *A, LMatrixCC *eigenvals);
  bool eigenvalues_hermitian(const LMatrixRR *A, LMatrixRR *eigenvals);
  bool eigenvalues_hermitian(const LMatrixCC *A, LMatrixRR *eigenvals);
  bool eigenvectors(const LMatrixRR *A, LMatrixCC *eigenvals, LMatrixCC *eigenvecs);
  bool eigenvectors(const LMatrixCC *A, LMatrixCC *eigenvals, LMatrixCC *eigenvecs);
  bool eigenvectors_hermitian(const LMatrixRR *A, LMatrixRR *eigenvals, LMatrixRR *eigenvecs);
  bool eigenvectors_hermitian(const LMatrixCC *A, LMatrixRR *eigenvals, LMatrixCC *eigenvecs);
  bool least_squares(const LMatrixRR *A, const LMatrixRR *B, LMatrixRR *X);
  bool least_squares(const LMatrixCC *A, const LMatrixCC *B, LMatrixCC *X);
  
  bool SVD(const LMatrixRRR *A,
    LMatrixRRR *Sigma,
    LMatrixRRR *U,
    LMatrixRRR *VT);
  bool SVD(const LMatrixCCC *A,
    LMatrixRRR *Sigma,
    LMatrixCCC *U,
    LMatrixCCC *VT);
  bool SVD_divide_conquer(const LMatrixRRR *A,
    LMatrixRRR *Sigma,
    LMatrixRRR *U,
    LMatrixRRR *VT);
  bool SVD_divide_conquer(const LMatrixCCC *A,
    LMatrixRRR *Sigma,
    LMatrixCCC *U,
    LMatrixCCC *VT);
  bool eigenvalues(const LMatrixRRR *A, LMatrixCCC *eigenvals);
  bool eigenvalues(const LMatrixCCC *A, LMatrixCCC *eigenvals);
  bool eigenvalues_hermitian(const LMatrixRRR *A, LMatrixRRR *eigenvals);
  bool eigenvalues_hermitian(const LMatrixCCC *A, LMatrixRRR *eigenvals);
  bool eigenvectors(const LMatrixRRR *A, LMatrixCCC *eigenvals, LMatrixCCC *eigenvecs);
  bool eigenvectors(const LMatrixCCC *A, LMatrixCCC *eigenvals, LMatrixCCC *eigenvecs);
  bool eigenvectors_hermitian(const LMatrixRRR *A, LMatrixRRR *eigenvals, LMatrixRRR *eigenvecs);
  bool eigenvectors_hermitian(const LMatrixCCC *A, LMatrixRRR *eigenvals, LMatrixCCC *eigenvecs);
  bool least_squares(const LMatrixRRR *A, const LMatrixRRR *B, LMatrixRRR *X);
  bool least_squares(const LMatrixCCC *A, const LMatrixCCC *B, LMatrixCCC *X);
}

#endif

/*
// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
*/

