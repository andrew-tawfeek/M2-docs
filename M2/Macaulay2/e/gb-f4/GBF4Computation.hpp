#pragma once

/**
 * @file gb-f4/GBF4Computation.hpp
 * @brief `newf4::GBF4Computation` --- top-level driver for the refactored F4 Gröbner-basis engine.
 *
 * Declares the class that owns all of the new F4's state: a
 * `MonomialHashTable` for basis monomials and a separate one
 * for S-pair LCMs, a `PolynomialList` of original inputs, a
 * `Basis` holding the evolving GB and its `GBPolyStatus`
 * flags, a `MonomialLookupTable` for fast divisor queries, an
 * `SPairSet` of pending overlaps, a `MacaulayMatrix` reused
 * each degree, and a reference to the user-selected
 * `VectorArithmetic` backend (`Zp`, `QQ` reconstruction, etc.).
 * `initializeWithMatrix` and `initializeWithBasicPolyList` are
 * the two ingestion entry points. The class itself is a plain
 * `class` --- it does not inherit from `GBComputation`; the
 * `GBComputation` virtuals are exposed through `GBF4Interface`,
 * which holds a `unique_ptr<GBF4Computation>` and forwards.
 *
 * Modern counterpart of the legacy `f4/f4-computation.hpp`:
 * same algorithmic shape (per-degree S-pair selection, build
 * the Macaulay matrix, row-reduce, promote new pivots to the
 * basis, prune subsumed S-pairs), but each concern lives in
 * its own class instead of being interleaved. All `gb-f4/`
 * classes live inside `namespace newf4` so their names don't
 * collide with the older F4. The `Strategy` enum,
 * forward-declared here, picks between variant choices (buffer
 * sizes, parallelism, monomial layout) and is defined in the
 * `.cpp`.
 *
 * @see Basis.hpp
 * @see MacaulayMatrix.hpp
 * @see MonomialHashTable.hpp
 * @see MonomialLookupTable.hpp
 * @see PolynomialList.hpp
 * @see SPairs.hpp
 * @see GBF4Interface.hpp
 */

#include "Basis.hpp"
#include "MacaulayMatrix.hpp"
#include "MonomialHashTable.hpp"
#include "MonomialLookupTable.hpp"
#include "PolynomialList.hpp"
#include "SPairs.hpp"
#include "../VectorArithmetic.hpp"

class FreeModule;

namespace newf4 {

enum class Strategy;
  
class GBF4Computation
{
 private:
  // Core Data:
  MonomialHashTable mBasisMonomials;     // table for monomials in the basis
  PolynomialList mInput; // as a MonomialHashTable.
  Basis mGBSoFar; // same hash table here.
  
  const FreeModule* mFreeModule;  // for debugging information *only*

  MonomialLookupTable mMonomialLookup;   // put this in Basis or not?
  MonomialHashTable mSPairMonomials;     // table for the LCMs of spairs
  SPairSet mSPairs;
  const VectorArithmetic& mVectorArithmetic;
  MacaulayMatrix mMacaulayMatrix;

  std::vector<int> mVariableWeights;

  // Support Data:
  // GB of base ring (for GBs over a quotient)
  // Hilbert function hint information
  // how to get spairs with exterior/skew variables
public:
  GBF4Computation(const VectorArithmetic& vectorArithmetic,
                  const FreeModule* freeModule,
                  const std::vector<int>& variableWeights,
                  Strategy strategy);

  void initializeWithMatrix(const Matrix* M);
  void initializeWithBasicPolyList(const BasicPolyList& basicPolyList);

  void dumpBasisMonomials() const;

  const PolynomialList& getInput() const { return mInput; }

  // these are all for debugging only -- copies made, etc.
  void showInput() const;
  void showGBStatusArray() const;
  void showMinimalBasis() const;
  void showFullBasis() const;

};

  // Steps:
  //  1. "Read in" the example (uses MatrixStream stuff)
  //     Set the ring information (variables names, #vars, charac, ??)
  //     Reads into a PolynomialList, which is in the Basis.
  //  2.    
// a separate computation class will contain the necessary stuff for
// computations over QQ, rational reconstruction, etc

} // end newf4 namespace

// Local Variables:
// indent-tabs-mode: nil
// End:
