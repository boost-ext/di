//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TEST_COMMON_VISITOR_HPP
#define BOOST_DI_TEST_COMMON_VISITOR_HPP

#include <boost/test/unit_test.hpp>
#include <typeinfo>
#include <vector>
#include <string>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>

namespace boost {
namespace di {
namespace test {
namespace common {

using namespace boost;
using namespace mpl;

template<typename TSequence = vector0<> >
class visitor
{
    typedef std::vector<std::string> visits_t;

public:
    ~visitor()
    {
        verify<TSequence>(0);
    }

    template<typename T> void operator()() const
    {
        visits.push_back(typeid(typename T::type).name());
    }

private:
    template<typename sequence> void verify(int, typename enable_if< empty<sequence> >::type* = 0) { }
    template<typename sequence> void verify(int i, typename disable_if< empty<sequence> >::type* = 0)
    {
        BOOST_CHECK_EQUAL(typeid(typename front<sequence>::type).name(), visits.at(i));
        verify<typename pop_front<sequence>::type>(i + 1);
    }

    mutable visits_t visits;
};

} // namespace common
} // namespace test
} // namespace di
} // namespace boost

#endif

