//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
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
    BOOST_DI_CTOR(c, int, double, char) { }
};

} // namespace

class creation_by_smart_ptr
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

public:
    typedef creation_by_smart_ptr is_policy;

    template<
        typename TDeps
      , typename TGiven
      , typename = void
    >
    struct verify : mpl::false_
    {
       BOOST_MPL_ASSERT_MSG(
            false
          , CREATION_NOT_BY_SMART_PTR_IS_DISALLOWED
          , (TGiven)
        );
    };

    template<typename TDeps, typename TGiven>
    struct verify<
        TDeps
      , TGiven
      , typename boost::enable_if<has_element_type<TGiven>>::type
    > : mpl::true_
    { };
};

int main() {
    di::injector<creation_by_smart_ptr> injector;
    injector.create<std::shared_ptr<c>>();
    //injector.create<c>(); //compile error (CREATION_NOT_BY_SMART_PTR_IS_DISALLOWED)

    return 0;
}

