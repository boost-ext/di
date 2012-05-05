//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_INSTANCE_HPP
#define BOOST_DI_AUX_INSTANCE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include "boost/di/aux/value_type.hpp"

namespace boost {
namespace di {
namespace aux {

template<
    typename T
  , typename TContext = mpl::vector0<>
  , typename = void
>
class instance
{
public:
    typedef instance type;
    typedef T element_type;

    explicit instance(const T& member)
        : member_(member)
    { }

    explicit instance(T& member)
        : member_(member)
    { }

    explicit instance(const shared_ptr<T>& member)
        : member_(member)
    { }

    variant<const T&, T&, shared_ptr<T> > get() const {
        return member_;
    }

private:
    variant<const T&, T&, shared_ptr<T> > member_;
};

template<
    typename T
  , typename TContext
>
class instance<
        T
      , TContext,
        typename enable_if<
            mpl::or_<
                is_same<typename aux::value_type<T>::type, std::string>
              , is_arithmetic<typename aux::value_type<T>::type>
            >
        >::type
    >
{
    typedef typename aux::value_type<T>::type value_type;

public:
    typedef instance type;
    typedef T element_type;

    explicit instance(value_type member)
        : member_(member)
    { }

    value_type get() const {
        return member_;
    }

private:
    value_type member_;
};

} // namespace aux
} // namespace di
} // namespace boost

#endif

