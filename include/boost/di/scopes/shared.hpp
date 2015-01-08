//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_SHARED_HPP
#define BOOST_DI_SCOPES_SHARED_HPP

#include "boost/di/wrappers/shared.hpp"

namespace boost { namespace di { namespace scopes {

class shared {
public:
    static constexpr auto priority = false;

    template<class, class T>
    class scope {
    public:
        template<class, class TProvider>
        auto create(const TProvider& provider) {
            using deleter = typename TProvider::deleter;
            if (!object_) {
                object_ = provider.get();
            }
            return wrappers::shared<T>{object_};
        }

    private:
        std::shared_ptr<T> object_;
    };
};

}}} // boost::di::scopes

#endif

