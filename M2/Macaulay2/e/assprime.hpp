/*!
    @class AssociatedPrimes
    @abstract   (description)
    @discussion (description)
*/

// (c) 1994 Michael E. Stillman
#ifndef _assprime_hh
#define _assprime_hh

/**
 * @file assprime.hpp
 * @brief `AssociatedPrimes` --- codimension and minimal-codimension associated primes of a monomial ideal.
 *
 * Declares `AssociatedPrimes`, a class that operates on a
 * (caller-supplied radical) `MonomialIdeal`. Its `state` enum
 * has two values, `do_codim` (cheap, computes the codimension)
 * and `do_primes` (expensive, enumerates the associated primes
 * of minimal codimension), so a caller can pay only for the
 * codimension and step the same instance through to the primes
 * later without restarting. The public entry points are
 * `codimension()`, `associated_primes(count)`,
 * `min_primes(maxcodim, count)`, and `max_indep_sets(count)`;
 * `count = -1` means "no limit". Results come back as a
 * `MonomialIdeal` whose generators are squarefree monomials,
 * each encoding one prime by the variables it contains (the
 * `max_indep_sets` output is the complementary squarefree
 * monomial for each min prime).
 *
 * Monomial associated-prime calculation avoids Groebner bases
 * and coefficient arithmetic entirely: it is the combinatorial
 * recursion `ass(I) = ass(I, x_i) U {(x_i) + ass(I : x_i^inf)}`
 * walking the `Nmi_node` tree of the input. Callers are
 * expected to have already radicalised the input.
 *
 * @see monideal.hpp
 */

#include "monideal.hpp"

class AssociatedPrimes
// A class which enables one to compute the codimension and
// associated primes of minimal codimension, of a monomial ideal.
{
  enum { do_codim, do_primes } state;
  int min_codim;
  int nvars;

  MonomialIdeal *mi;  // A radical monomial ideal

  MonomialIdeal *ass_primes;  // Assoc. primes of minimal codim, stored
                              // in a monideal, where each monomial
                              // corresponds to a prime monomial ideal
                              // whose generators are the variables occurring
                              // in the monomial.
  int minprime_limit;  // -1 means find all.  >= 1 means stop after that number
  int n_minprimes;  // number found so far, during associated_primes computation
  int **exps;

  void ass_prime_generator(Nmi_node *p, int codim);

 public:
  AssociatedPrimes(const MonomialIdeal *const &mi);
  AssociatedPrimes(const MonomialIdeal *const &mi, int codim);
  ~AssociatedPrimes();

  int codimension();
  MonomialIdeal *associated_primes(int count);

  MonomialIdeal *min_primes(int maxcodim, int count);
  // maxcodim == nvars means get all of them
  // count == -1 means no limit
  // return value: A monomial ideal where each
  //  monomial prime ideal is represented as a
  //  monomial: the product of the generators.
  // NOTE: this is the complement of the gens from
  //  max_indep_sets

  MonomialIdeal *max_indep_sets(int count);
};

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
