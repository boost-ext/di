//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <QDeps/QDeps.hpp>
#include "../QDeps/Test/Common/Data.hpp"

class PrintVisitor
{
public:
    template<typename T, typename TCallStack, typename TScope> void operator()() const
    {
        std::cout << typeid(T).name() << std::endl;
    }
};

using namespace QDeps::Front::Base;
using namespace QDeps::Test::Common;

struct VisitorModule : Module
    <
        Singletons <
            CIf0
        >
    >
{ };

int main()
{
    QDeps::Utility::Injector<VisitorModule> injector;
    injector.visit<C8>(PrintVisitor());

    return 0;
}

