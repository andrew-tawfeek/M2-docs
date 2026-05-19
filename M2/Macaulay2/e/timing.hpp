#ifndef __timing_hpp_
#define __timing_hpp_

/**
 * @file timing.hpp
 * @brief Inline `std::chrono::steady_clock` wrappers and elapsed-time conversion helpers.
 *
 * Provides `timer()` / `now()` (both returning a
 * `std::chrono::steady_clock::time_point`) for capturing
 * high-resolution timestamps, plus templated
 * `nanoseconds(duration)` / `microseconds(duration)` /
 * `seconds(duration)` helpers that pull the count out of any
 * duration type via `duration_cast`. Centralising the clock
 * choice in one header lets the engine swap clocks (mock during
 * tests, per-platform alternatives where the steady clock is
 * slow) without touching call sites and gives a single hook for
 * future per-thread CPU-time accounting.
 *
 * Heavy callers are `SLP-imp.hpp`'s NAG evaluator (per-step
 * timestamps that drive continuation-step heuristics), the F4
 * GB engines (per-reduction trace timings), and the time-limit
 * branch of the stop-condition machinery in `comp.hpp`.
 *
 * @see SLP-imp.hpp
 * @see comp.hpp
 */

#include <chrono>

inline std::chrono::steady_clock::time_point timer()
{
  return std::chrono::steady_clock::now();
}
inline std::chrono::steady_clock::time_point now()
{
  return std::chrono::steady_clock::now();
}

template <typename DurationType>
long nanoseconds(DurationType time_diff)
{
  return std::chrono::duration_cast<std::chrono::nanoseconds>(time_diff)
      .count();
}

template <typename DurationType>
long microseconds(DurationType time_diff)
{
  return std::chrono::duration_cast<std::chrono::microseconds>(time_diff)
      .count();
}

template <typename DurationType>
double seconds(DurationType time_diff)
{
  return 1.0e-9 *
         static_cast<double>(
             std::chrono::duration_cast<std::chrono::nanoseconds>(time_diff)
                 .count());
}

#define TIME(t, call)                  \
  {                                    \
    auto __now1 = now();               \
    call;                              \
    auto __now2 = now();               \
    t += nanoseconds(__now2 - __now1); \
  }

#endif
// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
