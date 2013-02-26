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
    {
        typedef di::generic_module<
            di::deduced<
                c0if0 // singleton<bind<if0, c0if0>>, per_request<c3>
            >
        > generic_module;

        di::injector<generic_module>().create<c4>();
    }

    return 0;
}

