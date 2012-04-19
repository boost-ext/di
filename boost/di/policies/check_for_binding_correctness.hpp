//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_CHECK_FOR_BINDING_CORRECTNESS_HPP
#define BOOST_DI_POLICIES_CHECK_FOR_BINDING_CORRECTNESS_HPP

#include <boost/mpl/bool.hpp>

namespace boost {
namespace di {
namespace policies {

class check_for_binding_correctness
{
public:
    template<
        typename TDeps
      , typename TGiven
      , typename Enable = void
    >
    class verify
        : public mpl::true_
    { };
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

