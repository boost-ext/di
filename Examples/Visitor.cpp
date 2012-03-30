//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <string>
#include <QDeps/QDeps.hpp>
#include "../QDeps/Test/Common/Data.hpp"
#include <cxxabi.h>

std::string demangle(const std::string & p_mangled)
{
    char* l_demangled = abi::__cxa_demangle(p_mangled.c_str(), 0, 0, 0);

    if (l_demangled)
    {
        boost::shared_ptr<char> l_result(l_demangled, std::free);
        return std::string(l_demangled);
    }

    return p_mangled;
}

class PrintVisitor
{
public:
    //template<typename TIf, typename TImpl, typename TCallStack, typename TScope> void operator()() const
    template<typename TImpl, typename TCallStack, typename TScope> void operator()() const
    {
        //tabs according to stack size
        std::cout << demangle(typeid(TImpl).name()) << std::endl;
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

