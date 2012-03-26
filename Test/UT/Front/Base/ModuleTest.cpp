//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include "Test/Common/Data.hpp"
#include "QDeps/Back/Dependency.hpp"
#include "QDeps/Front/Base/Module.hpp"

namespace QDeps
{
namespace Front
{
namespace Base
{
namespace UT
{

using namespace Test::Common;
using namespace boost::mpl;

struct MyModule : Module
    <
        //Scope<Back::Scope::Singleton>::Bind
        Singletons<
            CIf0,
            Bind<If0, CIf0>,
            Bind<int>::InCall<C1>
        >
        //Scope<Back::Scope::PerRequest>::Bind
        //<
            //Bind<If0, CIf0>
        //>
        //Externals::Bind<int, double>,
        //External<char>,
        //Externals::Bind<float>
    >
{ };

TEST(Module, One)
{
    //std::cout << abi::__cxa_demangle(typeid(MyModule::Externals::type).name(), 0, 0, 0) << std::endl;
    std::cout << abi::__cxa_demangle(typeid(MyModule::Dependencies::type).name(), 0, 0, 0) << std::endl;

    //EXPECT_TRUE((
        //equal
        //<
            //vector
            //<
                //int, double, char, float
            //>,
            //MyModule::Externals
        //>::value
    //));

    //EXPECT_TRUE((
        //equal
        //<
            //vector
            //<
                //Back::Dependency<Scopes::Singleton, CIf0, CIf0, vector0<>, boost::is_base_of<boost::mpl::_1, CIf0> >
            //>,
            //MyModule::Dependencies
        //>::value
    /*));*/
}

} // namespace UT
} // namespace Base
} // namespace Front
} // namespace QDeps

