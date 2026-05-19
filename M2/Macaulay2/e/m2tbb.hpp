#ifndef M2TBB_HPP
#define M2TBB_HPP

/**
 * @file m2tbb.hpp
 * @brief Engine TBB shim --- single point of inclusion for every parallel primitive.
 *
 * Brings in `mathicgb/mtbb.hpp` after consulting `M2/config.h` for
 * the configure-time `WITH_TBB` flag. When TBB is not available
 * the header defines `MATHICGB_NO_TBB`, which makes mathicgb's
 * abstractions (parallel `for`, atomic counters, mutexes,
 * flow-graph nodes) compile down to no-op serial code. Engine
 * code that wants parallelism must include this header rather
 * than `<tbb/...>` directly so that flipping `WITH_TBB` at
 * configure time is the only change required.
 *
 * Primary consumers are `schreyer-resolution/` (Schreyer-frame
 * scheduling and the dep-graph), `NCAlgebras/NCF4` (parallel row
 * reduction), and any future engine path that wants to opt in.
 *
 * @see schreyer-resolution/res-schreyer-frame.hpp
 * @see NCAlgebras/NCF4.hpp
 */

// The plan: All uses of TBB go through the following interface.

#include <M2/config.h>                    // make sure WITH_TBB is set before including mtbb.hpp

#ifndef WITH_TBB
#define MATHICGB_NO_TBB 1
#endif

#include "mathicgb/mtbb.hpp"

#endif
