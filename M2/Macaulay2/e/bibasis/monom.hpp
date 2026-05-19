/*****************************************************************************
 *   Copyright (C) 2006-2011 by Mikhail V. Zinin                             *
 *   mzinin@gmail.com                                                        *
 *                                                                           *
 *   You may redistribute this file under the terms of the GNU General       *
 *   Public License as published by the Free Software Foundation, either     *
 *   version 2 of the License, or any later version.                         *
 *****************************************************************************/

#ifndef BIBASIS_MONOM_HPP
#define BIBASIS_MONOM_HPP

/**
 * @file bibasis/monom.hpp
 * @brief `BIBasis::Monom` --- abstract squarefree-monomial base for the three Janet orderings.
 *
 * Declares the polymorphic root of the BIBasis monomial
 * hierarchy: a singly-linked list of `VarsListNode` records
 * (variable index plus `Next`) carrying a cached `TotalDegree`,
 * with the variable count `DimIndepend` shared across the
 * subsystem as a static. Each `VarsListNode` allocates from a
 * static `FastAllocator` slab so the millions of nodes the
 * prolongation loop creates stay cache-resident. Exponents are
 * stored as `short int`s --- in `F_2[x]/(x_i^2 - x_i)` every
 * exponent is 0 or 1, so the only thing the list needs to record
 * is the *set* of variables that appear.
 *
 * The pure virtuals `MultiplyBy`, `SetOne`, `operator[]`,
 * `FirstMultiVar`, and `GetVariablesSet` are filled in by the
 * three concrete subclasses --- `MonomLex`, `MonomDL`, `MonomDRL`
 * --- each implementing one monomial ordering. The bibasis
 * algorithm templates on the concrete type rather than dispatching
 * through `Monom*`, so the abstract surface here is mostly a
 * shared layout and a documentation anchor for the ordering
 * variants.
 *
 * @see allocator.hpp
 * @see monomLex.hpp
 * @see monomDL.hpp
 * @see monomDRL.hpp
 * @see polynom.hpp
 */

#include <set>
#include <iostream>
#include "allocator.hpp"

namespace BIBasis
{
    class Monom 
    {
    public:
        typedef short int Integer;
        enum Order
        {
            Lex,
            DegLex,
            DegRevLex
        };

    protected:
        struct VarsListNode
        {
            Integer Value;
            VarsListNode* Next;
            static FastAllocator Allocator;

            VarsListNode();
            ~VarsListNode();

            void* operator new(size_t);
            void operator delete(void* ptr);
        };
        VarsListNode* ListHead;
        Integer TotalDegree;

        static Integer DimIndepend;

    protected:
        Monom();
        virtual void MultiplyBy(Integer var) = 0;
        virtual VarsListNode* Find(const Integer var) const = 0;

    public:
        virtual ~Monom();
        virtual Integer Degree() const;

        virtual void SetOne() = 0;
        virtual Integer operator[](const Integer var) const = 0;
        
        virtual Integer FirstMultiVar() const = 0;
        virtual std::set<Integer> GetVariablesSet() const = 0;

        static Integer GetDimIndepend();
        static void SetDimIndepend(Integer independ);
    };

    inline Monom::VarsListNode::VarsListNode()
        : Value(0)
        , Next(nullptr)
    {
    }

    inline Monom::VarsListNode::~VarsListNode()
    {
    }

    inline void* Monom::VarsListNode::operator new(std::size_t)
    {
        return Allocator.Allocate();
    }

    inline void Monom::VarsListNode::operator delete(void* ptr)
    {
        Allocator.Free(ptr);
    }

    inline Monom::Monom()
        : ListHead(nullptr)
        , TotalDegree(0)
    {
    }

    inline Monom::~Monom()
    {
    }

    inline Monom::Integer Monom::Degree() const
    {
        return TotalDegree;
    }

    inline Monom::Integer Monom::GetDimIndepend()
    {
        return DimIndepend;
    }
}

#endif //BIBASIS_MONOM_HPP
