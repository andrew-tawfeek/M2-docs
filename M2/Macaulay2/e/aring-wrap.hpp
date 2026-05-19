// Copyright 2011 Michael E. Stillman

#ifndef _aring_wrap_hpp_
#define _aring_wrap_hpp_

/**
 * @file aring-wrap.hpp
 * @brief `RElementWrap<RingType>` --- templated wrapper that exposes one aring value as the abstract `RElement`.
 *
 * The aring framework resolves arithmetic by ring type at compile
 * time, but values that flow across the engine boundary (or land in
 * a `Matrix` or `Polynomial` slot) need a runtime base class because
 * the caller does not always know which aring produced them.
 * `RElement` is that base; `RElementWrap<RingType>` is the concrete
 * derivation that stores one `RingType::ElementType` value. The
 * `friend ARing::converter(...)` declaration is what allows
 * cross-ring coercion in `aring-translate.hpp` to extract or inject
 * the typed value during a translate call.
 *
 * This file is the element-level counterpart of `aring-glue.hpp`'s
 * ring-level `ConcreteRing<RingType>`: together they implement the
 * "wrap a templated implementation in an abstract base" pattern at
 * two scales (rings, and values inside rings). Reading them together
 * is the quickest way to see how the aring framework handles type
 * erasure at the engine boundary while preserving compile-time
 * specialisation inside hot paths.
 *
 * @see aring.hpp
 * @see aring-glue.hpp
 * @see aring-translate.hpp
 */

#include "aring.hpp"

namespace M2 {

/**
\ingroup rings
*/
template <class RingType>
class RElementWrap : public RElement
{
  friend bool ARing::converter(const ARing *sourceR,
                               const ARing *targetR,
                               const RElement &a,
                               RElement &b);

 public:
  typedef typename RingType::ElementType element_type;
  ~RElementWrap() {}
  RElementWrap() {}
  RElementWrap(const element_type &a) : val_(a) {}
  RElementWrap(const RElement &a)
      : val_(static_cast<const RElementWrap &>(a).val_)
  {
  }

 private:
  friend class AConcreteRing<RingType>;
  element_type val_;
};

/**
\ingroup rings
*/
template <class RingType>  // RingType should inherit from RingInterface
class AConcreteRing : public ARing
{
  friend bool ARing::converter(const ARing *sourceR,
                               const ARing *targetR,
                               const RElement &a,
                               RElement &b);

 public:
  typedef typename RingType::ElementType element_type;
  typedef RElementWrap<RingType> ringelem_type;

  AConcreteRing() {}
  AConcreteRing(RingType R) : R_(R) {}
  virtual RingID getRingID() const { return RingType::ringID; }
  RingType &getInternalRing() { return R_; }
  const RingType &getInternalRing() const { return R_; }
  virtual void init_set(RElement &a, long val) const
  {
    R_.init_set(RELEM(RingType, a), val);
  }

  virtual void add_to(RElement &a, const RElement &b) const
  {
    R_.add_to(RELEM(RingType, a), constRELEM(RingType, b));
  }

 private:
  RingType R_;
};
};

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e  "
// indent-tabs-mode: nil
// End:
