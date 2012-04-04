//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <typeinfo>
#include <vector>
#include <string>
#include <boost/mpl/vector.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include "QDeps/Back/Detail/Visitor.hpp"
#include "QDeps/Back/Scopes/PerRequest.hpp"
#include "Test/Common/Data.hpp"

namespace QDeps
{
namespace Back
{
namespace Detail
{
namespace UT
{

using namespace Test::Common;
using namespace Utility;
using namespace Back::Scopes;
using namespace Back::Aux;
using namespace boost::mpl;
using namespace boost;

template<typename TSeq>
class VisitorMock
{
    typedef std::vector<std::string> Visits;

public:

    ~VisitorMock()
    {
        verify<TSeq>(0);
    }

    template<typename T, typename TCallStack, typename TScope> void operator()() const
    {
        visits.push_back(typeid(T).name());
    }

private:
    template<typename Seq> void verify(int, typename boost::enable_if< boost::mpl::empty<Seq> >::type* = 0) { }
    template<typename Seq> void verify(int i, typename boost::disable_if< boost::mpl::empty<Seq> >::type* = 0)
    {
        EXPECT_EQ(typeid(typename boost::mpl::front<Seq>::type).name(), visits.at(i));
        verify<typename boost::mpl::pop_front<Seq>::type>(i + 1);
    }

    mutable Visits visits;
};

TEST(Visitor, BaseOfInterfaceNotTrivialCtor)
{
    VisitorMock
    <
        vector
        <
            TransactionUsage,
            shared_ptr< Provider<Transaction> >,
            shared_ptr<C3>,
            int
        >
    >
    visitor;

    Visitor
    <
        vector
        <
            Dependency<PerRequest, TransactionProvider, TransactionProvider, vector0<>, or_< is_base_of<_1, TransactionProvider>, is_same<_1, TransactionProvider> > >
        >
    >::execute<TransactionUsage, TransactionUsage, vector0<> >(visitor);
}

} // namespace UT
} // namespace Detail
} // namespace Back
} // namespace QDeps

