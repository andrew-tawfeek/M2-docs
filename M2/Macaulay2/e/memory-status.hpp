/**
 * @file memory-status.hpp
 * @brief Placeholder memory-statistics hooks: `memorystat1` / `memorystat2` / `memorystat3`.
 *
 * Declares three unimplemented `int`-returning functions whose
 * definitions in `memory-status.cpp` return distinct sentinel
 * constants (`123`, `1234`, `12345`). The names are referenced
 * from the interpreter side through the `.d` glue layer and
 * surface in M2's `memory()` builtin, but no real engine-wide
 * memory accounting has been wired in yet --- the sentinels are
 * a deliberate signal that "this stat is not implemented." The
 * file exists so the link line keeps working while the real
 * implementation is deferred.
 *
 * @see mem.hpp
 */

int memorystat1(void);
int memorystat2(void);
int memorystat3(void);
