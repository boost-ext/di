//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CIRCULAR_DEPENDENCIES_HPP
#define BOOST_DI_POLICIES_CIRCULAR_DEPENDENCIES_HPP

#include "boost/di/aux_/config.hpp"


namespace boost {
namespace di {
namespace policies {

/**
 *@code
 * using namespace di::policies;
 *
 * struct cd2;
 * struct cd1 { cd1(cd2*); };
 * struct cd2 { cd2(cd1*); };
 *
 * di::make_injector().create<cd1>(circular_dependencies()); // compile error
 * di::make_injector().create<int>(circular_dependencies()); // compile ok
 *@endcode
 */
class circular_dependencies
{
    template<typename TCallStack>
    struct is_unique_call_stack
        : aux::mpl::bool_<
              //static_cast<std::size_t>(aux::mpl::accumulate<
                  //typename aux::mpl::transform<
                      //TCallStack
                    //, aux::mpl::count<TCallStack, aux::mpl::_>
                  //>::type
                //, aux::mpl::int_<0>
                //, aux::mpl::plus<aux::mpl::_1, aux::mpl::_2>
              //>::type::value
            //) == aux::mpl::size<TCallStack>::value
          true
          >
    { };

public:
    template<typename T>
    void assert_policy() const {
       BOOST_DI_ASSERT_MSG(
            is_unique_call_stack<typename T::call_stack>::value
          , CIRCULAR_DEPENDENCIES_ARE_NOT_ALLOWED
          , typename T::call_stack
        );
    }
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

