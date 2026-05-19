//
// Created by Frank Moore on 7/5/23.
//

#pragma once

/**
 * @file gb-f4/MonomialLookupTable.hpp
 * @brief `newf4::MonomialLookupTable` --- divisibility-aware leading-term index for the new F4.
 *
 * Declares the lookup structure that lets the refactored F4
 * answer "does some basis leading monomial divide this target?"
 * without scanning every basis element. Each `MonomialInfo`
 * entry caches an `mIsUsed` flag, the monomial's simple degree,
 * a `MonomialMask` (one bit per variable set iff the exponent is
 * positive), an offset into the encoded-monomial pool, and the
 * `mValue` index of the owning polynomial. `findDivisor`,
 * `findAllDivisors`, and `findAllDivisees` pre-filter by
 * `maskDivides` (bitwise subset) and `simpleDegree` before
 * running the exact exponent-vector test, which is what makes
 * the linear walk affordable.
 *
 * Soft-delete is the maintenance strategy: when a basis element
 * is subsumed, `retire(monIndex)` just flips its `mIsUsed` bit
 * to false; the structure tolerates many tombstones before
 * `compactify()` reshuffles the storage. Complements
 * `MonomialHashTable` (which keys by exact equality) and acts as
 * the new-F4 successor to the legacy `montable.hpp` / mathic-
 * trait-based path.
 *
 * @see MonomialHashTable.hpp
 * @see MonomialView.hpp
 * @see MonomialTypes.hpp
 * @see GBF4Computation.hpp
 * @see SPairs.hpp
 * @see montable.hpp
 */

#include "MonomialTypes.hpp"
#include "MonomialView.hpp"
#include <vector>
#include <iostream>

namespace newf4 {

struct MonomialInfo {
  bool mIsUsed;            // whether to use for divisibility checks
  int mSimpleDegree;       // simple degree
  MonomialMask mMask;      // divisibility mask
  int mOffset;             // offset where monomial starts
  int mValue;              // index of polynomial associated to this MonomialInfo
};

class MonomialLookupTable
{
  friend class MonomialLookupIterator;
 private:
  std::vector<MonomialInt> mMonomialSpace;
  std::vector<MonomialInfo> mMonomialInfo;

  // statistics info

 public:

  inline static MonomialMask createMask(const MonomialView& monView)
  {
    MonomialMask result = 0;
    for (const auto vp : monView)
      result |= 1 << (vp.first & (sizeof(MonomialMask)*8 - 1));
    return result;
  }

  inline static bool maskDivides(MonomialMask divisor,
				 MonomialMask divisee)
  {
    auto result = divisor & (~divisee);
    return (result == 0);
  }

  class MonomialLookupIterator
  {
    using IterType = decltype(mMonomialInfo.cbegin());

   private:
    //TODO, needs to be added: const MonomialLookupTable& mTable;
    IterType mIter;

   public:
    MonomialLookupIterator(const MonomialLookupTable& table,
                           IterType iter)
        :
      //TODO: to be added: mTable(table),
          mIter(iter)
    {
      (void) table;
    }

   public:
    bool isUsed() { return mIter->mIsUsed; }
    int simpleDegree() { return mIter->mSimpleDegree; }
    uint64_t mask() { return mIter->mMask; }
    int value() { return mIter->mValue; }
    int offset() { return mIter->mOffset; }
  };

  // insert monomial(s) into table
  void insertMonomial(const MonomialView monView, int index);

  // compactify
  void compactify();

  // begin and end (both const and not)
  MonomialLookupIterator begin() const { return MonomialLookupIterator(*this,mMonomialInfo.begin()); }
  MonomialLookupIterator end() const { return MonomialLookupIterator(*this,mMonomialInfo.end()); }

  // find divisor
  int findDivisor(const MonomialView monView) const;

  // find divisors
  std::vector<int> findAllDivisors(const MonomialView monView) const;
  void findAllDivisors(const MonomialView monView, 
		       std::vector<int>& result,
		       bool stopAtOne = false) const;

  // find divisees
  std::vector<int> findAllDivisees(const MonomialView monView) const;
  void findAllDivisees(const MonomialView monView,
		       std::vector<int>& result) const;

  // retire monomial(s)
  void retire(int monIndex) { mMonomialInfo[monIndex].mIsUsed = false; }
  std::vector<int> retireAllDivisees(const MonomialView monView);
  void retireAllDivisees(const MonomialView monView,
			 std::vector<int>& result);

  // display
  std::ostream& display(std::ostream& ostr) const;

  // memoryUsed
  long memoryUsed() const;
};

}  // namespace newf4
