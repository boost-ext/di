//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_EXTENSIONS_DEPENDENCY_CROSS_PLATFORM_CALL_HPP
#define BOOST_DI_EXTENSIONS_DEPENDENCY_CROSS_PLATFORM_CALL_HPP

namespace boost { namespace di { inline namespace v1 { namespace extensions { namespace dependency {

/**
 * Supports for bind<i, impl>() when using variable templates
 */
struct cross_platform_call {
    template<class TDependency>
    struct extension {
        #if !defined(BOOST_DI_MSVC)
        const TDependency& operator()() const noexcept {
            return static_cast<const TDependency&>(*this);
        }
        #endif
    };
};

#endif

}}}}} // boost::di::v1::extensions::dependency

