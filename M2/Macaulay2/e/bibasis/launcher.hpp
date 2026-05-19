/*****************************************************************************
 *   Copyright (C) 2006-2011 by Mikhail V. Zinin                             *
 *   mzinin@gmail.com                                                        *
 *                                                                           *
 *   You may redistribute this file under the terms of the GNU General       *
 *   Public License as published by the Free Software Foundation, either     *
 *   version 2 of the License, or any later version.                         *
 *****************************************************************************/

#ifndef BIBASIS_LAUNCHER_HPP
#define BIBASIS_LAUNCHER_HPP

/**
 * @file bibasis/launcher.hpp
 * @brief `BIBasis::Launcher` --- monomial-order dispatcher between the engine boundary and `BooleanInvolutiveBasis`.
 *
 * Declares the single `Launcher` class that the engine's
 * `rawBIBasis` entry calls into. `GetBIBasisMatrix(matrix,
 * toGroebner)` validates the input via `CheckMatrix`, reads the
 * polynomial ring's monomial ordering off the `Matrix`, and
 * instantiates `BooleanInvolutiveBasis<MonomLex>`,
 * `<MonomDL>`, or `<MonomDRL>` accordingly --- one
 * specialisation per supported ordering, so the inner-loop
 * comparators inline without virtual dispatch.
 *
 * Centralising the template instantiations here keeps the engine
 * boundary free of BIBasis-specific template noise: every other
 * translation unit only sees `Launcher::GetBIBasisMatrix`, and
 * the heavy templated machinery is compiled exactly once per
 * order inside `launcher.cpp`.
 *
 * @see involutive.hpp
 * @see monom.hpp
 * @see monomLex.hpp
 * @see monomDL.hpp
 * @see monomDRL.hpp
 */

#include "involutive.hpp"
#include "matrix.hpp"

namespace BIBasis
{
    class Launcher
    {
    public:
        const Matrix* GetBIBasisMatrix(const Matrix* matrix, int toGroebner) const;
    
    private:
        bool CheckMatrix(const Matrix* matrix) const;
    };
}

#endif // BIBASIS_LAUNCHER_HPP
