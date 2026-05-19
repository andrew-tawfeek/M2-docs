// Copyright 2015 Anton Leykin and Mike Stillman

// Anton Leykin's code in this file is in the public domain.

#ifndef _slp_hpp_
#define _slp_hpp_

/**
 * @file SLP.hpp
 * @brief Public umbrella header for the straight-line-program evaluator used by NAG.
 *
 * A straight-line program is a DAG whose nodes are inputs,
 * constants, and arithmetic operations (`+`, `*`, occasionally `-`,
 * `/`, `^`, scalar negation, square root) with tagged outputs.
 * `NAG.cpp` compiles a polynomial system to one of these once and
 * then evaluates it at every step of a homotopy continuation path,
 * sharing common subexpressions across all outputs and reusing the
 * forward graph in a backward pass to obtain partial derivatives for
 * Newton's method. The evaluation pass is a single forward sweep ---
 * cache-friendly compared to re-walking a polynomial AST.
 *
 * The implementation is split across three files so the same code
 * can be instantiated over `double`, `mpfr_t` (RRR), `acb_t` (CCC),
 * and other numerical backends: `SLP-defs.hpp` declares the node
 * types and value-type templates, `SLP-imp.hpp` provides the
 * template implementations of evaluation and differentiation, and
 * `SLP.cpp` carries the non-template glue and the registry.
 *
 * @see NAG.hpp
 * @see SLP-defs.hpp
 * @see SLP-imp.hpp
 */

#include "mutablemat-defs.hpp"
#include "SLP-defs.hpp"
#include "SLP-imp.hpp"
#include "mutablemat-imp.hpp"

#include "buffer.hpp"
#include "matrix.hpp"
#include "aring-glue.hpp"

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
