#ifndef __F4toM2interface_h_
#define __F4toM2interface_h_

/**
 * @file f4/f4-m2-interface.hpp
 * @brief `F4toM2Interface` --- static translators between engine `vec` / `Matrix` and F4's `GBF4Polynomial`.
 *
 * Declares the static-only `F4toM2Interface` namespace-class
 * that converts each direction across the F4 boundary:
 * `from_M2_vec` (and the matrix variant) packs an engine
 * `Matrix` column into a `GBF4Polynomial` for the F4 inner
 * loop; the inverse `to_M2_vec` unpacks F4 output back into the
 * engine's `vec` shape, and `poly_set_degrees` reads the
 * weight-system degree of an F4 polynomial for sugar tracking
 * and `DegreeLimit =>` accounting. Every method threads through
 * a `VectorArithmetic *` (the per-coefficient-ring fast path)
 * and a `MonomialInfo *` (the F4 packed-monomial layout) so the
 * helpers stay stateless.
 *
 * `f4-computation.hpp` reaches into these routines on every
 * call across the boundary, both when feeding the F4 engine and
 * when extracting its results.
 *
 * @see f4-computation.hpp
 * @see f4-types.hpp
 * @see moninfo.hpp
 */

#include "interface/m2-types.h"  // for M2_arrayint
#include "f4/f4-types.hpp"   // for gb_array, GBF4Polynomial (ptr only), coefficient_m...
#include "ringelem.hpp"      // for vec

class FreeModule;
class VectorArithmetic;
class Matrix;
class MonomialInfo;
class MutableMatrix;

class F4toM2Interface
{
 public:
  static void poly_set_degrees(const VectorArithmetic* VA,
                               const MonomialInfo *MI,
                               const M2_arrayint wts,
                               const GBF4Polynomial &f,
                               int &deg,
                               int &alpha);

  static void from_M2_vec(const VectorArithmetic* VA,
                          const MonomialInfo *MI,
                          const FreeModule *F,
                          vec v,
                          GBF4Polynomial &result);

  static vec to_M2_vec(const VectorArithmetic* VA,
                       const MonomialInfo *MI,
                       const GBF4Polynomial &f,
                       const FreeModule *F);

  static void from_M2_matrix(const VectorArithmetic* VA,
                             const MonomialInfo *MI,
                             const Matrix *m,
                             gb_array &result_polys);

  static Matrix *to_M2_matrix(const VectorArithmetic* VA,
                              const MonomialInfo *MI,
                              gb_array &polys,
                              const FreeModule *F);

  static MutableMatrix *to_M2_MutableMatrix(const VectorArithmetic* VA,
                                            coefficient_matrix *mat,
                                            gb_array &gens,
                                            gb_array &gb);
};

#endif

// Local Variables:
//  compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
//  indent-tabs-mode: nil
//  End:
