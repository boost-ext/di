//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_SHARED_HPP
#define BOOST_DI_SCOPES_SHARED_HPP

#include <memory>
#include "boost/di/aux_/compiler.hpp"
#include "boost/di/wrappers/shared.hpp"

namespace boost { namespace di { inline namespace v1 { namespace scopes {

class shared {
public:
    template<class, class T>
    class scope {
    public:
        template<class T_>
        using is_referable = typename wrappers::shared<T>::template is_referable<T_>;

        template<class, class TProvider>
        auto try_create(const TProvider& provider)
            -> decltype(wrappers::shared<T>{std::shared_ptr<T>{provider.get()}});

        template<class, class TProvider>
        auto create(const TProvider& provider) {
            if (BOOST_DI_UNLIKELY(!object_)) {
                object_ = std::shared_ptr<T>{provider.get()};
            }
            return wrappers::shared<T>{object_};
        }

    private:
        std::shared_ptr<T> object_;
    };
};

}}}} // boost::di::v1::scopes

#endif


//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <string>
#include "boost/di/scopes/shared.hpp"
#include "common/fakes/fake_provider.hpp"

namespace boost { namespace di { inline namespace v1 { namespace scopes {

test create_shared = [] {
    shared::scope<int, int> shared;

    std::shared_ptr<int> object1 = shared.create<int>(fake_provider<int>{});
    std::shared_ptr<int> object2 = shared.create<int>(fake_provider<int>{});

    expect_eq(object1, object2);
};

}}}} // boost::di::v1::scopes

