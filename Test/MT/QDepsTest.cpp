//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "Test/MT/MT.hpp"
#include "Test/Common/Ctors.hpp"
#include "QDeps/Utility/Injector.hpp"
#include "QDeps/Utility/Scopes/Singleton.hpp"
#include "QDeps/Front/Generic/Module.hpp"

namespace QDeps
{
namespace MT
{

using namespace boost::mpl;
using namespace Utility;
using namespace Test::Common;

MT
(
    struct Module : Front::Generic::Module
    {
        typedef vector
        <
            Impl<If0, CIf0>
        >
        Binding;
    };
)
{
    //Utility::Injector<Module> inj;
    //boost::shared_ptr<C8> c8 = inj.create< boost::shared_ptr<C8> >();
}}

} // namespace MT
} // namespace QDeps

