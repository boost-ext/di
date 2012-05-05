//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <typeinfo>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/int.hpp>
#include <boost/di.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

namespace {

struct name { };
struct double_name { };

struct i0 { virtual ~i0() { }; };

struct c0 : i0
{
    BOOST_DI_CTOR(c0, di::named<int, name>, double) { }
};

struct c01 : i0 { };

struct c1
{
    BOOST_DI_CTOR(c1, boost::shared_ptr<i0>) { }
};

struct c2
{
    BOOST_DI_CTOR(c2, c1, int, double, char) { }
};

struct c3 { };
struct c4 { };

struct c5
{
    BOOST_DI_CTOR(c5, c1, c2, boost::shared_ptr<i0>, boost::shared_ptr<c3>, boost::shared_ptr<c4>) { }
};

} // namespace

struct visitor
{
    template<typename T>
    void operator()() const {
        std::cout << typeid(typename T::type).name() << std::endl;
    }
};

int main()
{
    boost::shared_ptr<c3> c3_(new c3);
    c4 c4_;

    {
        typedef di::generic_module<
            di::singletons<
                c1
            >
          , di::per_requests<
                c0
              , mpl::int_<42>
              , di::bind<int, mpl::int_<87> >::in_name<name>
              , di::bind<c01>::in_call<di::call_stack<c2, c1> >
            >
          , di::externals<
                double
              , di::annotate<di::bind<double>::in_call<c0> >::with<double_name>
              , c3
              , c4
            >
        > generic_module_t;

        generic_module_t generic_module( //note: order is not important
            generic_module_t::set<double>(42.0)
          , generic_module_t::set<double_name>(87.0)
          , generic_module_t::set<c3>(c3_)
          , generic_module_t::set<c4>(c4_)
        );

        generic_module.create<c5>();
        generic_module.visit<c5>(visitor());
    }

    {
        struct generic_module_
            : di::generic_module<
                  di::singletons<
                      c1
                  >
                , di::per_request<
                      c0
                  >
                , di::scope<di::scopes::per_request>::bind<
                      mpl::int_<42>
                  >
                , di::per_requests<
                      di::bind<int, mpl::int_<87> >::in_name<name>
                    , di::bind<c01>::in_call<di::call_stack<c2, c1> >
                  >
                , di::external<
                      double
                  >
                , di::externals<
                      di::annotate<di::bind<double>::in_call<c0> >::with<double_name>
                    , c3
                    , c4
                  >
              >
        {
            generic_module_(double d1
                          , double d2
                          , boost::shared_ptr<c3> c3_
                          , c4& c4_ )
                : generic_module(
                      set<double>(d1)
                    , set<double_name>(d2)
                    , set<c3>(c3_)
                    , set<c4>(c4_)
                  )
            { }
        } generic_module(42.0, 87.0, c3_, c4_);

        generic_module.create<c5>();
        generic_module.visit<c5>(visitor());
    }
}

