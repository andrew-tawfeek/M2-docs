#pragma once

/**
 * @file gb-f4/Basis.hpp
 * @brief `newf4::Basis` --- evolving Gröbner basis container with `GBPolyStatus` per element.
 *
 * Declares the `Basis` class that holds the current Gröbner
 * basis (plus the original generators) while the refactored F4
 * driver runs. Each element pairs a `Polynomial` (held inside a
 * `PolynomialList` bound to a shared `MonomialHashTable` and
 * `VectorArithmetic`) with a `GBPolyStatus` flag --- `Gen`,
 * `MinGen`, `MinGB`, `NonMinGB`, or `Retired` --- tracking how
 * the element relates to the input ideal and to the running
 * basis. The reporters the M2 interpreter polls (`get_mingens`,
 * `get_gb`, `get_change`) read directly off these flags.
 *
 * Modern counterpart of the legacy `f4/`-era `gb_array`: it
 * still tracks "what's in the basis right now," but the
 * monomial side is keyed through `MonomialHashTable` indices
 * rather than packed-monomial pointers. Single-writer ---
 * parallelism in the new F4 lives inside the matrix-reduction
 * phase rather than inside basis updates --- so no TBB
 * synchronisation lives in this header.
 *
 * @see MonomialHashTable.hpp
 * @see PolynomialList.hpp
 * @see GBF4Computation.hpp
 * @see MacaulayMatrix.hpp
 */

#include "MonomialHashTable.hpp"
#include "PolynomialList.hpp"
#include "../VectorArithmetic.hpp"

namespace newf4 {

// not a final choice of statuses
enum class GBPolyStatus { Gen, MinGen, MinGB, NonMinGB, Retired };

inline std::string toString(GBPolyStatus status) {
   switch (status) {
      case GBPolyStatus::Gen: return "Gen";
      case GBPolyStatus::MinGen: return "MinGen";
      case GBPolyStatus::MinGB: return "MinGB";
      case GBPolyStatus::NonMinGB: return "NonMinGB";
      case GBPolyStatus::Retired: return "Retired";
   };
   return "Undefined";
}

// Basis: keeping the Groebner basis (and original generators)
class Basis
{
private:
  std::vector<GBPolyStatus> mGBStatusList;
  PolynomialList mPolynomialList;

public:
  Basis(const VectorArithmetic& VA,
        MonomialHashTable& monHashTable)
    : mPolynomialList(VA, monHashTable)
  {
  }

  ~Basis() = default;

  const PolynomialList& getPolynomialList() const { return mPolynomialList; }
  const VectorArithmetic& vectorArithmetic() const { return mPolynomialList.vectorArithmetic(); }
  const std::vector<GBPolyStatus>& getGBStatusList() const { return mGBStatusList; }

  // What functionality do we need here?
  // wipe out a poly from basis
  // add poly to the basis (and adjust other elements whose lead terms are div by the new poly)
  // test divisibility both ways (monomial divisibility class)

  // accessor functions

};

}

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
