//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_MEMORY_HPP
#define BOOST_DI_AUX_MEMORY_HPP

#include <memory>
#include <boost/shared_ptr.hpp>

namespace boost {
namespace di {
namespace aux {
    using ::std::unique_ptr;
    using ::std::shared_ptr;
    using ::std::weak_ptr;
} // namespace aux

namespace aux_ {
    using ::boost::shared_ptr;
} // namespace aux_

} // namespace boost
} // namespace di

#endif

