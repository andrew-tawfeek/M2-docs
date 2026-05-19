// Copyright 2013  Michael E. Stillman

#ifndef _dmat_zzp_flint_hpp_
#define _dmat_zzp_flint_hpp_

/**
 * @file dmat-zzp-flint.hpp
 * @brief `DMat<M2::ARingZZpFlint>` --- dense `Z/p` matrices stored in FLINT `nmod_mat_t`.
 *
 * Specialises the dense-matrix template for the FLINT-backed Z/p
 * aring. Entries live in a single `nmod_mat_t` --- an array of
 * `mp_limb_t` representatives in `[0, p)` plus the precomputed
 * reciprocal FLINT uses for Barrett-style fast modular reduction.
 * Arithmetic, rank, LU, determinant, inverse, and null-space all
 * dispatch into `nmod_mat_*` so the inner loop never performs an
 * integer division and the heavy linear-algebra work runs in
 * FLINT's cache-aware blocked routines.
 *
 * The engine ships a sibling FFLAS-FFPACK specialisation in
 * `dmat-zzp-ffpack.hpp`. Dispatch (in `mat-linalg.hpp`) picks
 * between them by matrix shape and prime size: small matrices with
 * larger primes prefer FLINT because BLAS startup cost dominates;
 * larger matrices with small primes prefer FFPACK because it can
 * reinterpret entries as `double` and dispatch into BLAS. As with
 * every FLINT-backed header, `M2/gc-include.h` precedes the FLINT
 * include so allocations route through bdwgc.
 *
 * @see dmat.hpp
 * @see dmat-lu-zzp-flint.hpp
 * @see dmat-lu-zzp-ffpack.hpp
 * @see aring-zzp-flint.hpp
 */

#include <utility>              // for swap
#include "aring-zzp-flint.hpp"  // for ARingZZpFlint

// The following needs to be included before any flint files are included.
#include <M2/gc-include.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#include <flint/nmod_mat.h>  // for nmod_mat_t, nmod_mat_clear, nmod_mat_init, nmod_m...
#pragma GCC diagnostic pop

template <typename ACoeffRing>
class DMat;

//////////////////////////////////////////////////////////////
// Flint: use nmod_mat for implementation of dense matrices //
//////////////////////////////////////////////////////////////

template <>
class DMat<M2::ARingZZpFlint>
{
 public:
  typedef M2::ARingZZpFlint ACoeffRing;
  typedef ACoeffRing CoeffRing;
  typedef ACoeffRing::ElementType ElementType;
  typedef ElementType elem;
  typedef ACoeffRing::Element Element;

  DMat() : mRing(0) {}
  DMat(const ACoeffRing& R, size_t nrows, size_t ncols) : mRing(&R)
  {
    nmod_mat_init(mArray, nrows, ncols, R.characteristic());
  }

  DMat(const DMat<ACoeffRing>& M) : mRing(&M.ring())
  {
    nmod_mat_init_set(mArray, M.mArray);
  }

  ~DMat() { nmod_mat_clear(mArray); }

  // swap the actual matrices of 'this' and 'M'.
  // The rings must be the same.
  void swap(DMat<ACoeffRing>& M)
  {
    std::swap(mRing, M.mRing);
    std::swap(mArray, M.mArray);
  }

  const ACoeffRing& ring() const { return *mRing; }
  size_t numRows() const { return nmod_mat_nrows(mArray); }
  size_t numColumns() const { return nmod_mat_ncols(mArray); }

  ElementType& entry(size_t row, size_t column)
  {
    return nmod_mat_entry(mArray, row, column);
  }
  const ElementType& entry(size_t row, size_t column) const
  {
    return nmod_mat_entry(mArray, row, column);
  }

  void resize(size_t new_nrows, size_t new_ncols)
  {
    DMat newMatrix(ring(), new_nrows, new_ncols);
    swap(newMatrix);
  }

  // These are labelled 'unsafe', as it s possible the rows
  // are out of order (which happens in particular if
  // certain flint functions created this.
  const ElementType* unsafeArray() const { return mArray->entries; }
  ElementType*& unsafeArray() { return mArray->entries; }
 public:
  // Access routines so that the flint nmod_mat interface may be used
  const nmod_mat_t& nmod_mat() const { return mArray; }
  nmod_mat_t& nmod_mat() { return mArray; }
 private:
  const ACoeffRing* mRing;
  nmod_mat_t mArray;
};

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
