//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FWD_EXT_HPP
#define BOOST_DI_FWD_EXT_HPP

#if defined(_LIBCPP_VERSION)                               // __pph__
#define NAMESPACE_STD_BEGIN _LIBCPP_BEGIN_NAMESPACE_STD {  // __pph__
#else                                                      // __pph__
#define NAMESPACE_STD_BEGIN namespace std {                // __pph__
#endif                                                     // __pph__

#if defined(_LIBCPP_VERSION)                         // __pph__
#define NAMESPACE_STD_END _LIBCPP_END_NAMESPACE_STD  // __pph__
#else                                                // __pph__
#define NAMESPACE_STD_END }                          // __pph__
#endif                                               // __pph__

// libc++
#if __has_include(<__config>)  // __pph__
#include <__config>            // __pph__
#endif                         // __pph__

#if __has_include(<memory>)  // __pph__
#include <memory>            // __pph__
#else                        // __pph__
NAMESPACE_STD_BEGIN
template <class>
class shared_ptr;
template <class>
class weak_ptr;
template <class, class>
class unique_ptr;
NAMESPACE_STD_END
#endif                       // __pph__

#if __has_include(<vector>)  // __pph__
#include <vector>            // __pph__
#else                        // __pph__
NAMESPACE_STD_BEGIN
template <class, class>
class vector;
NAMESPACE_STD_END
#endif                       // __pph__

#if __has_include(<set>)  // __pph__
#include <set>            // __pph__
#else                     // __pph__
NAMESPACE_STD_BEGIN
template <class, class, class>
class set;
NAMESPACE_STD_END
#endif                    // __pph__

#if __has_include(<initializer_list>)  // __pph__
#include <initializer_list>            // __pph__
#else                                  // __pph__
NAMESPACE_STD_BEGIN
template <class>
class initializer_list;
NAMESPACE_STD_END
#endif                                 // __pph__

#if __has_include(<tuple>)  // __pph__
#include <tuple>            // __pph__
#else                       // __pph__
NAMESPACE_STD_BEGIN
template <class...>
class tuple;
NAMESPACE_STD_END
#endif                      // __pph__

#if __has_include(<iterator>)  // __pph__
#include <iterator>            // __pph__
#else                          // __pph__
NAMESPACE_STD_BEGIN
template <class>
class move_iterator;
NAMESPACE_STD_END
#endif                         // __pph__

#if __has_include(<string>)  // __pph__
#include <string>            // __pph__
#else                        // __pph__
NAMESPACE_STD_BEGIN
template <class>
struct char_traits;
NAMESPACE_STD_END
#endif                       // __pph__

// clang-format off
#if __has_include(<boost/shared_ptr.hpp>)  // __pph__
// clang-format on
#include <boost/shared_ptr.hpp>  // __pph__
#else                            // __pph__
namespace boost {
template <class>
class shared_ptr;
}  // namespace boost
#endif                           // __pph__

#undef NAMESPACE_STD_BEGIN  // __pph__
#undef NAMESPACE_STD_END    // __pph__

#endif
