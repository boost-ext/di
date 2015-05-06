//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_INJECTOR_HPP
#define BOOST_DI_FAKE_INJECTOR_HPP

#include "common/fakes/fake_config.hpp"

namespace boost { namespace di { inline namespace v1 {

template<class TExpected = void>
struct fake_injector : fake_config<> {
    using deps = void;

    template<class T, class TName = no_name, class TIsRoot = std::false_type>
    struct is_creatable
        : std::true_type
    { };

    template<class T>
    auto create() const noexcept {
        return create_impl<T>();
    }

    template<class T>
    auto create_impl() const noexcept {
        return T{};
    }
};

}}} // boost::di::v1

#endif

