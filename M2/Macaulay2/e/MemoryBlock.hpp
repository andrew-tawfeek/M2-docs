#ifndef __memory_block_hpp__
#define __memory_block_hpp__

/**
 * @file MemoryBlock.hpp
 * @brief Bump-pointer arena allocator for transient inner-loop allocations.
 *
 * `MemoryBlock` is a thin wrapper around `memt::Arena` from the
 * `memtailor` submodule. All allocations come out of a single growing
 * buffer; `allocateArray<T>(n)` returns the next `n * sizeof(T)`
 * bytes, bumps the cursor, and hands back a `(begin, end)` pair.
 * Individual allocations cannot be freed --- the whole arena is
 * released when the `MemoryBlock` is destroyed. That matches the
 * "millions of tiny objects allocated during one GB step, then all
 * thrown away" shape of the engine's hot paths in `f4/`, `gb-f4/`,
 * `schreyer-resolution/`, and `NCAlgebras/`.
 *
 * Memory comes from the system heap, not bdwgc, so anything stored
 * inside a `MemoryBlock` must be plain-old-data --- the garbage
 * collector will not scan these pages. The underlying `allocArrayNoCon`
 * also skips per-element constructor calls; callers either initialise
 * the bytes explicitly or treat them as raw storage (encoded
 * monomials, exponent vectors, S-pair records).
 *
 * @see ExponentList.hpp
 * @see ExponentVector.hpp
 */

#include <memtailor/Arena.h>  // for Arena
#include <iostream>           // for operator<<, endl, basic_ostream, cout
#include <utility>            // for pair

class MemoryBlock
{
public:
  MemoryBlock() { mArena = new memt::Arena; }
  ~MemoryBlock() { delete mArena; }
  //MemoryBlock(MemoryBlock&& source) : mArena(std::move(source.mArena)) {}

  //MemoryBlock& operator=(MemoryBlock&& source) {
    // mArena = std::move(source.mArena);
    // return (*this);
  //}

  template<typename T>
  std::pair<T*, T*> allocateArray(size_t nelems)
  {
    return mArena->allocArrayNoCon<T>(nelems);
  }

  template<typename T, typename MutexType>
  std::pair<T*, T*> safeAllocateArray(size_t nelems, MutexType& mutex)
  {
    typename MutexType::scoped_lock myLock(mutex);
    return mArena->allocArrayNoCon<T>(nelems);
  }

  template<typename T>
  std::pair<T*, T*> shrinkLastAllocate(T* begin, T* end, T* newtop)
  {
    mArena->freeTopArray(begin, end);
    std::pair<T*, T*> result = mArena->allocArrayNoCon<T>(newtop - begin);
    if (result.first != begin) std::cout << "ooops: location changed" << std::endl;
    return result;
  }

  template<typename T>
  void freeTopArray(T* begin, T*end)
  {
    mArena->freeTopArray(begin,end);
  }

  void deallocateAll()
  {
    mArena->freeAllAllocs();
  }

  void swap(MemoryBlock& source) {
    memt::Arena* temp = mArena;
    mArena = source.mArena;
    source.mArena = temp;
  }

  size_t getMemoryUsedInBytes() { return mArena->getMemoryUse(); } 

private:
  memt::Arena* mArena;
};
  
#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e  "
// indent-tabs-mode: nil
// End:
