/*****************************************************************************
 *   Copyright (C) 2006-2011 by Mikhail V. Zinin                             *
 *   mzinin@gmail.com                                                        *
 *                                                                           *
 *   You may redistribute this file under the terms of the GNU General       *
 *   Public License as published by the Free Software Foundation, either     *
 *   version 2 of the License, or any later version.                         *
 *****************************************************************************/

#ifndef BIBASIS_PCOMPARATOR_HPP
#define BIBASIS_PCOMPARATOR_HPP

/**
 * @file bibasis/pcomparator.hpp
 * @brief `BIBasis::PointerLessComparator` / `PointerMoreComparator` --- pointer-deref comparators for STL containers.
 *
 * Two tiny templated function objects that the involutive engine
 * passes to `std::sort` and `std::set` so collections of
 * `Polynom*` (or `Triple*`) order by the pointed-to value rather
 * than the raw pointer address. `PointerLessComparator` forwards
 * to `T::operator<`; `PointerMoreComparator` forwards to
 * `T::operator>` for descending sorts. Both throw a descriptive
 * `std::string` on a null pointer so the algorithm fails loudly
 * rather than dereferencing junk.
 *
 * Primary consumer is `BooleanInvolutiveBasis::ReduceSet`, which
 * sorts the working `GBasis` list by leading monomial before
 * incremental reduction; the rest of the subsystem reuses the
 * pair generically whenever it needs to sort pointer-owned
 * polynomials without copying.
 *
 * @see polynom.hpp
 * @see involutive.hpp
 * @see triple.hpp
 */

#include <string>
#include "error.h"

namespace BIBasis
{
    template<class T> class PointerLessComparator
    {
    public:
        bool operator() (const T* const& p1, const T* const& p2) 
        {
            if (!p1 || !p2)
            {
                throw std::string("BIBasis::PointerLessComparator::operator(): at least one argument is NULL.");
            }
            return *p1 < *p2;
        }
    };

    template<class T> class PointerMoreComparator
    {
    public:
        bool operator() (const T* const& p1, const T* const& p2) 
        {
            if (!p1 || !p2)
            {
                throw std::string("BIBasis::PointerMoreComparator::operator(): at least one argument is NULL.");
            }
            return *p1 > *p2;
        }
    };
}

#endif //BIBASIS_PCONPARATOR_HPP
