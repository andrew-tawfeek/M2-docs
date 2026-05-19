#pragma once

/**
 * @file gb-f4/MonomialTypes.hpp
 * @brief `Index`, `MonomialIndex`, `MonomialInt`, `ComponentIndex`, `HashInt`, `MonomialMask` --- new-F4 typed integer vocabulary.
 *
 * Declares the strongly-named integer aliases (all backed by
 * `int32_t` or `uint64_t`) that the rest of `namespace newf4`
 * uses everywhere in place of bare `int`. `Index` numbers GB
 * basis elements; `MonomialIndex` keys into the
 * `MonomialHashTable` with `0` reserved as the "undefined"
 * sentinel; `MonomialInt` is one word of the encoded monomial
 * stream and also the type used for cached degrees;
 * `ComponentIndex` numbers free-module components; `HashInt` is
 * the 64-bit hash returned by `MonomialHashFunction`; and
 * `MonomialMask` is the 64-bit bitset that
 * `MonomialLookupTable` uses for cheap divisibility pre-tests.
 *
 * The five aliases all compile to the same hardware types, but
 * the distinct C++ names let the compiler reject calls that mix
 * a `MonomialIndex` with an `Index` --- bugs that would
 * silently misbehave if every parameter were just an `int`. Every
 * other header in `gb-f4/` ultimately includes this one.
 *
 * @see MonomialHashTable.hpp
 * @see MonomialLookupTable.hpp
 * @see MonomialView.hpp
 * @see PolynomialList.hpp
 */

#include <cstdint>

namespace newf4 {

// Index of a GB element
using Index = int32_t;

// Index of a monomial in a monomial hash table.
// 0 is a sentinel, meaning that something that usually would
// point to a monomial is currently undefined.  Valid values are > 0.
using MonomialIndex = int32_t;

// Data type of the underlying monomial store.
// A monomial is a sequence of MonomialInts
// Also used to store degree of a monomial.
using MonomialInt = int32_t;

// Number indicating the free module component
using ComponentIndex = int32_t;

// Value of hashing a monomial (which may or may not include the component)
using HashInt = uint64_t;

// Used for divisibility masks in MonomialLookupTable
using MonomialMask = uint64_t;

}// end namespace f4


// Local Variables:
// indent-tabs-mode: nil
// End:
