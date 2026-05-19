/**
 * @file BasicPolyListParser.hpp
 * @brief Parsers from text (string or file) into a `BasicPolyList`, including Msolve input format.
 *
 * Three free functions read polynomial systems from text and return a
 * portable `BasicPolyList`: `parseMsolveFromString` /
 * `parseMsolveFile` consume the input format of the external
 * [Msolve](https://msolve.lip6.fr/) solver (variables and
 * characteristic come from dedicated header lines), and
 * `parseBasicPolyListFromString` consumes the engine's own
 * human-readable `ideal:` block format given an explicit variable
 * list. The free-function shape reflects that parsers carry no state
 * across calls --- text in, polynomials out.
 *
 * Because `BasicPolyList` is ring-agnostic, these parsers run before
 * any `PolynomialRing` is constructed, making them suitable for early
 * pipeline stages: test-input wiring, the `gb-f4` streaming
 * `PolynomialList` consumer, and the round-trip half of Msolve
 * dispatch.
 *
 * @see BasicPoly.hpp
 * @see BasicPolyList.hpp
 */

// This class implements parsing of polynomials from a string or file
// as well as Msolve format.
#pragma once


#include <string>
#include <vector>

#include "BasicPolyList.hpp"

BasicPolyList parseMsolveFromString(std::string contents); // requires Msolve header to determine variables, etc.
BasicPolyList parseMsolveFile(std::string filename);

BasicPolyList parseBasicPolyListFromString(std::string contents, std::vector<std::string> varnames);

// Local Variables:
// indent-tabs-mode: nil
// End:
