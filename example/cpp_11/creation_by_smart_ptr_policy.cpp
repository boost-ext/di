//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/di.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

namespace {

struct c
{
    c(int, double, char) { }
};

} // namespace

class creation_by_smart_ptr_policy
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

    template<typename TDependency, typename = void>
    struct is_creation_by_smart_ptr
        : mpl::true_
    { };

    template<typename TDependency>
    struct is_creation_by_smart_ptr<
        TDependency
      , typename boost::enable_if_c<mpl::size<typename TDependency::call_stack>::value == 1>::type
    >
        : has_element_type<typename TDependency::type>
    { };

public:
    template<typename TDependency>
    static typename boost::enable_if<is_creation_by_smart_ptr<TDependency>>::type assert_policy() { }

    template<typename TDependency>
    static typename boost::disable_if<is_creation_by_smart_ptr<TDependency>>::type assert_policy() {
        BOOST_DI_ASSERT_MSG(
            false
          , CREATION_NOT_BY_SMART_PTR_IS_DISALLOWED
          , (TDependency)
        );
    }
};

int main() {
    di::injector<> injector;
    injector.create<std::shared_ptr<c>>(creation_by_smart_ptr_policy());
    //injector.create<c>(creation_by_smart_ptr_policy()); //compile error (CREATION_NOT_BY_SMART_PTR_IS_DISALLOWED)

    return 0;
}

