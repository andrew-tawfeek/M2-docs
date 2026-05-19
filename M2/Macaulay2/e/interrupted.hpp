// Copyright 2012  Michael E. Stillman
#ifndef __interrupted__hpp__
#define __interrupted__hpp__

/**
 * @file interrupted.hpp
 * @brief `system_interrupted()` --- thread-safe polling predicate for Ctrl+C handling.
 *
 * Exposes the single global function `system_interrupted()`, a
 * cheap atomic read of the thread-local `interrupts_interruptedFlag`
 * the system supervisor sets in response to `SIGINT`. Engine inner
 * loops poll this at roughly one-millisecond cadence and bail out
 * to `COMP_INTERRUPTED` when the flag is true, the chain that
 * delivers a user Ctrl+C to a running computation. The actual
 * definition (in `interrupted.cpp`) is one wrapped macro call ---
 * the file exists so callers see only the abstract predicate
 * without having to pull in the supervisor and atomic-primitive
 * headers.
 *
 * Polled by every long-running engine loop --- the GB and
 * resolution algorithms, NAG path tracking, F4 matrix passes,
 * the dense linear-algebra routines --- as the cooperative
 * cancellation handshake matching the stop-condition machinery
 * declared in `comp.hpp`.
 *
 * @see comp.hpp
 */

extern bool system_interrupted();

#endif

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
