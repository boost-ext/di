//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/shared_ptr.hpp>
#include <boost/mpl/string.hpp>
#include <boost/mpl/int.hpp>
#include <boost/di.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

namespace {

struct name1 { };
struct name2 { };
struct name3 { };

struct i { virtual ~i() { }; };

struct impl : i { };
struct impl1 : i { };
struct impl2 : i { };
struct impl3 : i { };
struct impl4 : i { };

struct c0 { };

struct c1
{
    BOOST_DI_CTOR(c1, int, di::named<double, name1>) { }
};

struct c2
{
    BOOST_DI_CTOR(c2, di::named<double, name1>, c1) { }
};

struct c3
{
    BOOST_DI_CTOR(c3
      , std::string
      , boost::shared_ptr<i>
      , di::named<boost::shared_ptr<i>, name2>
    ) { }
};

struct c4 { };
struct c5 { };
struct c6
{
    BOOST_DI_CTOR(c6, boost::shared_ptr<i>) { }
};

struct c7
{
    BOOST_DI_CTOR(c7, di::named<boost::shared_ptr<i>, name3>) { }
};

struct c8
{
    BOOST_DI_CTOR(c8, std::string, boost::shared_ptr<i>, c7) { }
};

struct c9 { };

struct c10
{
    BOOST_DI_CTOR(c10, c9, c8) { }
};

struct c11
{
    BOOST_DI_CTOR(c11, c9, c10) { }
};

struct c12
{
    BOOST_DI_CTOR(c12, c9, c11) { }
};

struct c13
{
    BOOST_DI_CTOR(c13, boost::shared_ptr<c12>) { }
};

} // namespace

int main()
{
    {
        typedef di::generic_module<
            c0 // deduce
          , di::bind<int, mpl::int_<42>>::in_call<c1>
          , di::singleton<
                c1
            >
          , di::singleton<
                c3, c4
            >
          , di::per_request<
                c5, c6
            >
          , di::external<
                int
            >
          , di::external<
                double
              , di::annotate<di::bind<double>::in_call<c1, c2>>::with<name1>
            >
          , di::per_request<
                impl
              , mpl::string<'s'>
              , di::bind<i, impl1>
              , di::bind<i, impl2>::in_name<name2>
              , di::bind<i, impl3>::in_call<c6>
              , di::bind<i, impl4>::in_call<di::call_stack<c8, c7>>::in_name<name3>
            >
          , di::singleton<
                di::bind<c9>::in_call<c10, c11, di::call_stack<c13, c12>>
            >
        > generic_module;
    }

    {
        BOOST_AUTO(fusion_module, di::fusion_module<>()(
            di::bind<c0>() // per_request
          , di::bind<int, mpl::int_<42>>::in_call<c1>() // per_request
          , di::singleton<
                c1
            >()
          , di::per_request<
                c2
            >()
          , di::singleton<
                c3, c4
            >()
          , di::per_request<
                c5, c6
            >()
          , di::per_request<
                impl
              , mpl::string<'s'>
              , di::bind<i, impl1>
              , di::bind<i, impl2>::in_name<name2>
              , di::bind<i, impl3>::in_call<c6>
              , di::bind<i, impl4>::in_call<di::call_stack<c8, c7>>::in_name<name3>
            >()
          , di::singleton<
                di::bind<c9>::in_call<c10, c11, di::call_stack<c13, c12>>
            >()
          , di::bind<int>::to(0)
          , di::bind<double>::to(0.0)
          , di::bind<double>::in_call<c1, c2>::to(0.0)
        ));
    }
}

