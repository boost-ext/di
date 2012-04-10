//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_TEST_COMMON_VISITOR_HPP
#define QDEPS_TEST_COMMON_VISITOR_HPP

#include <typeinfo>
#include <vector>
#include <string>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>

namespace QDeps
{
namespace Test
{
namespace Common
{

using namespace boost;
using namespace boost::mpl;

template<typename TSeq = vector0<> >
class Visitor
{
    typedef std::vector<std::string> Visits;

public:
    ~Visitor()
    {
        verify<TSeq>(0);
    }

    template<typename T> void operator()() const
    {
        visits.push_back(typeid(typename T::Type).name());
    }

private:
    template<typename Seq> void verify(int, typename enable_if< empty<Seq> >::type* = 0) { }
    template<typename Seq> void verify(int i, typename disable_if< empty<Seq> >::type* = 0)
    {
        BOOST_CHECK_EQUAL(typeid(typename front<Seq>::type).name(), visits.at(i));
        verify<typename pop_front<Seq>::type>(i + 1);
    }

    mutable Visits visits;
};

} // namespace Common
} // namespace Test
} // namespace QDeps

#endif

