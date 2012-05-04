//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/di/policy.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

class check_for_creation_by_smart_ptr;

namespace boost {
namespace di {

template<>
struct defaults<detail::policy, specialized>
{
    typedef policy<
        policies::check_for_circular_dependencies
      , check_for_creation_by_smart_ptr
    > type;
};

} // namespace di
} // namespace boost

#include <boost/di.hpp>

namespace {

struct c
{
    BOOST_DI_CTOR(c, int i, double d, char c) { }
};

} // namespace

class check_for_creation_by_smart_ptr
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

public:
    template<
        typename TDeps
      , typename TGiven
      , typename = void
    >
    class verify
        : public mpl::false_
    {
       BOOST_MPL_ASSERT_MSG(
            false
          , CREATION_BY_SMART_PTR_IS_NOT_ALLOWED
          , (TGiven)
        );
    };

    template<
        typename TDeps
      , typename TGiven
    >
    class verify<TDeps, TGiven, typename boost::enable_if<has_element_type<TGiven> >::type>
        : public mpl::true_
    { };
};

int main()
{
    {
        di::injector<> injector;
        injector.create<boost::shared_ptr<c> >();
        //injector.create<c>(); //compiler error (CREATION_SMART_PTR_IS_ALLOWED)
    }

    {
        di::injector<di::policy<check_for_creation_by_smart_ptr> > injector;
        injector.create<boost::shared_ptr<c> >();
        //injector.create<c>(); //compiler error (CREATION_SMART_PTR_IS_ALLOWED)
    }

    return 0;
}

