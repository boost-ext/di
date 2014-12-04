//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOS_DI_POLICIES_ALLOW_TYPES_HPP
#define BOOS_DI_POLICIES_ALLOW_TYPES_HPP

namespace boost { namespace di { inline namespace policies {

template<class T>
struct is_bound
{ };

struct allow_types {
    template<class T>
    void operator()(const T& data) const noexcept {
        //using type = typename T::type;
        //struct not_resolved { };
        //auto dep = data.template resolve<type, not_resolved>();
    }
};

}}} // namespace boost::di::policies

#endif

