//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <boost/mpl/size.hpp>
#include <QDeps/QDeps.hpp>
#include "Common/Utility.hpp"
#include "Common/Data.hpp"

namespace Base    = QDeps::Front::Base;
namespace Utility = QDeps::Utility;

class PrintVisitor
{
public:
    template<typename T> void operator()() const
    {
        int size = boost::mpl::size<typename T::Context>::value;
        while(--size) {
            std::cout << "\t";
        }
        std::cout << demangle(typeid(typename T::Type).name()) << std::endl;
    }
};

struct VisitorModule : Base::Module
    <
        Base::Singletons <
            CIf0
        >
    >
{ };

int main()
{
    Utility::Injector<VisitorModule> injector;
    injector.visit<C8>(PrintVisitor());

    return 0;
}

