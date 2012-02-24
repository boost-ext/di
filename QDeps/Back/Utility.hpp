//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_UTILITY_HPP
#define QDEPS_BACK_UTILITY_HPP

#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/size.hpp>

namespace QDeps
{
namespace Back
{

template<typename T> struct GetDependency
{
    typedef typename T::Dependency type;
};

template<typename T> struct GetContext
{
    typedef typename T::Context type;
};

template<typename T> struct GetContextSize
{
    static const std::size_t value = boost::mpl::size<typename GetContext<T>::type>::value;
};

template<typename T> struct HasValue
{
    typedef typename T::HasValue type;
};

template<typename T> struct RemoveAccessors
{
    typedef typename boost::remove_pointer<typename boost::remove_cv<typename boost::remove_reference<T>::type>::type>::type type;
};

} // namespace Back
} // namespace QDeps

#endif

