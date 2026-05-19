// (c) 1994 Michael E. Stillman
#ifndef _text_io_hh_
#define _text_io_hh_

/**
 * @file text-io.hpp
 * @brief Text-formatting helpers layered on `buffer`: bignum print, line wrapping, `M2_gbTrace`-gated emit.
 *
 * Provides the engine-wide text utilities every output path
 * reaches for: `bignum_text_out(buffer&, mpz_srcptr)` renders
 * an `mpz_t` integer with M2's display conventions (no
 * thousands separator, leading minus for negatives, no leading
 * plus), `emit_wrapped(s)` prints to stdout with continuation
 * lines prefixed by the `wrapping_prefix` macro `"   -- "` so
 * long output stays visually unambiguous, and the
 * `(prlevel, s)` overload checks `M2_gbTrace >= prlevel` first
 * to gate tracing output behind the global verbosity dial.
 * `emit` and `emit_line` are the simpler stderr variants for
 * unconditional diagnostic messages.
 *
 * Pair with `buffer.hpp` for the underlying append-only text
 * accumulator. The `i_text_io()` initialiser hooks up the
 * column / wrap-width bookkeeping at startup.
 *
 * @see buffer.hpp
 */

#include "buffer.hpp"
#include "engine-includes.hpp"

extern int i_text_io();

#define wrapping_prefix "   -- "

void bignum_text_out(buffer &o, mpz_srcptr a);

void clear_emit_size();
void emit_wrapped(const char *s);
inline void emit_wrapped(int prlevel, const char *s)
{
  if (M2_gbTrace >= prlevel) emit_wrapped(s);
}

void emit(const char *s);       // print onto stderr, or cerr.
void emit_line(const char *s);  // print onto stderr, or cerr.
#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
