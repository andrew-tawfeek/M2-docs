// Copyright 2014-2016  Michael E. Stillman
#pragma once

/**
 * @file util.hpp
 * @brief Conversion helpers between M2 boundary types and standard C++ containers.
 *
 * Provides the small bidirectional converters every engine
 * boundary needs: `string_std_to_M2` / `string_M2_to_std`
 * marshal between `std::string` and `M2_string` (the engine's
 * length-prefixed string type), and the matching
 * `M2_arrayint_to_stdvector` / `stdvector_to_M2_arrayint` pair
 * convert between `std::vector<int>` and `M2_arrayint` so the
 * engine can use modern C++ containers internally while still
 * speaking the older C-style boundary types when crossing the
 * interpreter line. The helpers are header-inlined where
 * possible so the call sites stay free.
 *
 * @see interface/m2-types.h
 */

#include <string>   // for string, basic_string
#include <vector>   // for vector
#include <iostream> // for ostream

#include "interface/m2-mem.h"              // for getmemarraytype
#include "interface/m2-types.h"  // for M2_* types

/**
 * Utilities for converting between M2 types and standard C++ types
 */

inline M2_string string_std_to_M2(const std::string& s)
{
  // The function M2_tostringn does not modify s.data()...
  return M2_tostringn(const_cast<char*>(s.data()), static_cast<int>(s.size()));
}

inline std::string string_M2_to_std(const M2_string s)
{
  return std::string((char*)s->array, s->len);
}

inline void M2_ArrayString_to_stdvector(M2_ArrayString strs,
                                        std::vector<std::string>& result)
{
  result.reserve(strs->len);
  for (auto i = 0; i < strs->len; i++)
    result.push_back(string_M2_to_std(strs->array[i]));
}

inline std::vector<std::string> M2_ArrayString_to_stdvector(M2_ArrayString strs)
{
  std::vector<std::string> result;
  M2_ArrayString_to_stdvector(strs, result);
  return result;
}

inline M2_ArrayString stdvector_to_M2_ArrayString(
    const std::vector<std::string>& strs)
{
  // needed since M2_ArrayString len field is int
  auto len = static_cast<int>(strs.size());
  M2_ArrayString a = getmemarraytype(M2_ArrayString, len);
  for (auto i = 0; i < len; i++) a->array[i] = M2_tostring(strs[i].c_str());
  a->len = len;
  return a;
}

template <typename T>
inline M2_arrayint stdvector_to_M2_arrayint(const std::vector<T>& v)
{
  M2_arrayint result = M2_makearrayint(static_cast<int>(v.size()));
  for (auto i = 0; i < v.size(); i++) result->array[i] = static_cast<int>(v[i]);
  return result;
}

// TODO: can this be combined with the above?
template <typename T>
inline M2_arrayint stdvector_to_M2_arrayint(const gc_vector<T>& v)
{
  M2_arrayint result = M2_makearrayint(static_cast<int>(v.size()));
  for (auto i = 0; i < v.size(); i++) result->array[i] = static_cast<int>(v[i]);
  return result;
}

template <typename T>
inline std::vector<T> M2_arrayint_to_stdvector(M2_arrayint arr)
{
  std::vector<T> result;
  result.reserve(arr->len);
  for (auto i = 0; i < arr->len; i++) result.emplace_back(arr->array[i]);
  return result;
}

template <typename T>
inline std::ostream& operator<<(std::ostream& o, const std::vector<T>& ts)
{
  o << "[";
  for (auto a : ts)
    o << a << " ";
  o << "]";
  return o;
}

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// indent-tabs-mode: nil
// End:
