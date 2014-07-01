//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[concepts_cpp_11
//````C++11```
//<-
#include <boost/shared_ptr.hpp>
#include <boost/mpl/string.hpp>
//->
#include <boost/di.hpp>

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

struct c1 { c1(int, di::named<double, name1>) { } };
struct c2 { c2(di::named<double, name1>, c1) { } };
struct c3
{
    c3(std::string
     , std::shared_ptr<i>
     , const di::named<boost::shared_ptr<i>, name2>&
    ) { }
};
struct c4 { };
struct c5 { };
struct c6 { c6(std::shared_ptr<i>) { } };
struct c7 { c7(di::named<std::shared_ptr<i>, name3>) { } };
struct c8 { c8(std::string, std::shared_ptr<i>, c7) { } };
struct c9 { };
struct c10 { c10(c9, c8) { } };
struct c11 { c11(c9, c10) { } };
struct c12 { c12(c9, c11) { } };
struct c13 { c13(std::shared_ptr<c12>) { } };

} // namespace

int main() {
    using injector_t = di::injector<
        c0
      , di::bind_int<42>::when<di::call_stack<c1>>
      , di::bind_string<boost::mpl::string<'s'>>
      , di::shared<
            c1
        >
      , di::shared<
            c3, c4
        >
      , di::unique<
            impl
          , di::bind<i, impl1>
          , di::bind<i, impl2>::named<name2>
          , di::bind<i, impl3>::when<di::call_stack<c6>>
          , di::bind<i, impl4>::when<di::call_stack<c8, c7>>::named<name3>
        >
      , di::shared<
            di::bind<c9>::when<di::call_stack<c10>, di::call_stack<c11>, di::call_stack<c13, c12>>
        >
    >;

    auto injector = di::make_injector(
        injector_t()
      , di::unique<c5>()
      , di::bind<int>::to(42)
      , di::bind<double>::to(42.0)
      , di::bind<double>::when<di::call_stack<c1>, di::call_stack<c2>>::to(87.0)
    );
}

//`[table
//`[[Full code example: [@example/cpp_11/concepts.cpp concepts.cpp]]]]
//]

