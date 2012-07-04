//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/shared_ptr.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/string.hpp>
#include <boost/di.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

namespace {

struct if0
{
    virtual ~if0() { }
    virtual void dummy() = 0;
};

struct c0if0 : if0
{
    virtual void dummy() { }
};

struct c3
{
    BOOST_DI_CTOR(explicit c3, boost::shared_ptr<if0> /*singleton*/) { }
};

struct c4
{
    BOOST_DI_CTOR(c4, std::auto_ptr<c3> /*per_request*/) { }
};

} // namespace

int main()
{
    //scope_traits - auto deduction
    //by ref -> external
    //by shared_ptr -> singleton
    //by value -> per_request
    //by rvalue -> per_request
    //by const ref -> per_request
    //by ptr -> per_request
    //by const ptr -> per_request
    //by auto_ptr -> per_request
    //by unique_ptr -> per_request

    {
        typedef di::generic_module<
            c0if0
            //deduce<c0if0>
            //deduce<c1, c2>
            //per_request<c3>  //auto deduction
            //singleton<c0if0> //auto deduction
        > generic_module;

        di::injector<generic_module>().create<c4>();

/*        auto fu<>()(*/
          /*deduce<c1, c2, c3>()*/

    }

    return 0;
}


