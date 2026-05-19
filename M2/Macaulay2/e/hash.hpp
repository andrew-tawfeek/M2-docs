// Copyright 2002-2016  Michael E. Stillman
#ifndef _hash_hh_
#define _hash_hh_

/**
 * @file hash.hpp
 * @brief `EngineObject` / `MutableEngineObject` --- shared bases that supply the hash an M2 interpreter object expects.
 *
 * Declares two parallel bases:
 *
 * - `EngineObject` derives from `our_new_delete` (so allocation
 *   routes through bdwgc), supplies a virtual destructor anchor
 *   that makes `delete base_ptr` safe across subclass
 *   boundaries, and stores a `mutable` `mHashValue` that `hash()`
 *   fills lazily by calling the pure-virtual
 *   `computeHashValue()` on first read. The 0-to-1 bump after
 *   the call keeps "not yet computed" distinguishable from a
 *   legitimate hash of zero. Subclasses (immutable types like
 *   `Matrix`, `FreeModule`, ...) implement `computeHashValue`
 *   over their own stable state.
 *
 * - `MutableEngineObject` derives from `our_gc_cleanup` (so its
 *   destructor runs under a GC finaliser) rather than from
 *   `EngineObject`, and takes a different approach to hashing:
 *   no lazy cache, no `computeHashValue` hook. Every instance
 *   is assigned a unique sequential id from the static counter
 *   `mNextMutableHashValue` at construction time, and `hash()`
 *   just returns that id --- mutating the object never changes
 *   it. This is what `MutableMatrix` and other mutable engine
 *   types inherit from.
 *
 * The hashes are the cache keys M2's interpreter `HashTable`
 * uses to identify engine objects within a session; `std::hash<T>`
 * is unsuitable because engine objects are too large to hash in
 * full per lookup.
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
