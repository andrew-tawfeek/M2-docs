// Copyright 1995  Michael E. Stillman

#ifndef _FreeModule_hh_
#define _FreeModule_hh_

/**
 * @file freemod.hpp
 * @brief `FreeModule` --- finite-rank free module `R^n`, the type-level anchor for every `Matrix`.
 *
 * Declares `FreeModule`, an `EngineObject` that holds its base
 * ring, its rank `n`, a degree vector for each generator (entries
 * in the ring's degree monoid), and optionally a `SchreyerOrder`.
 * Every `Matrix` stores pointers to a source and target
 * `FreeModule`, so this class is what gives matrix shapes their
 * static guarantees --- degree compatibility and base-ring
 * agreement are checked through these pointers. The class is
 * intentionally immutable: every transformation (`sub_space`,
 * `direct_sum`, ...) returns a fresh `FreeModule`.
 *
 * When a Schreyer order is installed, the standard
 * `compare(e_i * m, e_j * m')` first compares
 * `m_{i} * m` against `m_{j} * m'` using the stored leading
 * monomials of the inducing generators and then tiebreaks by
 * index. This is what lets a syzygy module remember the order
 * structure of the resolution step it came from --- essential for
 * keeping leading-term arithmetic local to a homological degree
 * in `schreyer-resolution/`. The standalone `#undef FreeModule`
 * at the top of the header defuses a stray MinGW macro of the
 * same name.
 *
 * @see ring.hpp
 * @see schorder.hpp
 * @see matrix.hpp
 */

#include "ring.hpp"
#include "schorder.hpp"

class Matrix;
class GBMatrix;

// an include file under mingw32 defines a macro with the name FreeModule:
#undef FreeModule

class FreeModule : public EngineObject
{
  friend class Ring;
  friend class ResF4toM2Interface;
  FreeModule(const Ring *R, int n, bool has_schreyer_order);

 protected:
  gc_vector<monomial> components;  // Degrees of each component
  SchreyerOrder *schreyer;  // NULL, if not a Schreyer order...

  const Ring *R;

 protected:
  virtual unsigned int computeHashValue() const;

  void initialize(const Ring *RR);

  virtual FreeModule *new_free() const;

 public:
  static FreeModule *make_schreyer(const Matrix *m);
  static FreeModule *make_schreyer(const GBMatrix *m);

  Matrix *get_induced_order() const;

  virtual ~FreeModule();

 public:
  void append(const_monomial d);
  void append_schreyer(const_monomial d,
                       const_monomial base_monom,
                       int compare_num);  // append to a Schreyer order.
  // WARNING: change_degree modifies the degree, and should only be used during
  // the construction of a free module (or matrix).
  void change_degree(int i, const_monomial deg);

 public:
  const Ring *get_ring() const { return R; }
  const SchreyerOrder *get_schreyer_order() const { return schreyer; }
  const_monomial degree(int i) const { return components[i]; }
  int rank() const { return components.size(); }
  int primary_degree(int i) const;

  bool is_equal(const FreeModule *F) const;

  FreeModule *sub_space(int n) const;
  FreeModule *sub_space(M2_arrayint a) const;
  FreeModule *transpose() const;
  FreeModule *direct_sum(const FreeModule *G) const;
  FreeModule *shift(const_monomial d) const;
  FreeModule *tensor(const FreeModule *G) const;
  FreeModule *schur(const_monomial m) const;
  FreeModule *exterior(int p) const;
  FreeModule *symm(int p) const;

  M2_arrayintOrNull select_by_degrees(M2_arrayintOrNull lo,
                                      M2_arrayintOrNull hi) const;

  void direct_sum_to(const FreeModule *G);
  int lowest_primary_degree() const;
  int highest_primary_degree() const;

  void text_out(buffer &o) const;
};

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
