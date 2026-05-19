// Copyright 2002-2016  Michael E. Stillman
#ifndef _hash_hh_
#define _hash_hh_

/**
 * @file hash.hpp
 * @brief `EngineObject` / `MutableEngineObject` --- shared base for every long-lived engine class.
 *
 * Declares `EngineObject`, the common base that every long-lived
 * engine type derives from. It routes allocation through bdwgc
 * (via `our_new_delete`), supplies the virtual destructor anchor
 * that makes `delete base_ptr` safe across subclass boundaries,
 * and stores a lazily computed `mHashValue` filled by the
 * subclass's `computeHashValue()` on first read; the 0-to-1 bump
 * keeps "not yet computed" distinguishable from a legitimate
 * hash of zero. `MutableEngineObject` extends the base with the
 * discipline that any mutation resets the cache, so mutable
 * objects (`MutableMatrix`, GB intermediate state, ...) still
 * obey the lazy-hash invariant.
 *
 * The hashes are the cache keys M2's interpreter `HashTable`
 * uses to identify engine objects across sessions; each subclass
 * implements `computeHashValue` over a stable subset of its
 * state (e.g. `Matrix` hashes its source/target plus column
 * entries). `std::hash<T>` is unsuitable because engine objects
 * are too large to hash in full per lookup.
 *
 * @see newdelete.hpp
 */

#include "newdelete.hpp"
#include <cassert>
#include <M2/gc-include.h>

//class EngineObject : public gc
class EngineObject : public our_new_delete
{
 private:
  mutable unsigned int mHashValue;
 public:
  EngineObject() : mHashValue(0) {}

  virtual ~EngineObject() { /* nothing to do here */ }

  unsigned int hash() const
  { 
    if (mHashValue == 0) 
      {
        mHashValue = computeHashValue();
        if (mHashValue == 0) mHashValue = 1;
      }
    return mHashValue;
  }

 protected:
  virtual unsigned int computeHashValue() const = 0;
};

class MutableEngineObject : public our_gc_cleanup
{
 private:
  static unsigned int mNextMutableHashValue;
  unsigned int mHashValue;

 public:
  MutableEngineObject() : mHashValue(mNextMutableHashValue++) {}
  virtual ~MutableEngineObject() { /* nothing to do here */ }
  unsigned int hash() const { return mHashValue; }
};

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
