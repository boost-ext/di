//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_SINGLETON_HPP
#define BOOST_DI_SCOPES_SINGLETON_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/wrappers/shared.hpp"

namespace boost { namespace di { inline namespace v1 { namespace scopes {

std::false_type has_shared_ptr__(...);

template<class T>
auto has_shared_ptr__(T&&) -> aux::is_valid_expr<decltype(std::shared_ptr<T>{})>;

class singleton {
public:
    template<class, class T, class = decltype(has_shared_ptr__(std::declval<T>()))>
    class scope {
    public:
        template<class T_>
        using is_referable = typename wrappers::shared<T*>::template is_referable<T_>;

        template<class, class TProvider>
        static decltype(wrappers::shared<T*>{std::declval<TProvider>().get()})
        try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider& provider) {
            return create_impl(provider);
        }

    private:
        template<class TProvider>
        auto create_impl(const TProvider& provider) {
            static struct scoped_ptr { aux::owner<T*> ptr; ~scoped_ptr() noexcept { delete ptr; } } object{provider.get()};
            return wrappers::shared<T*>{object.ptr};
        }
    };

    template<class _, class T>
    class scope<_, T, std::true_type> {
    public:
        template<class T_>
        using is_referable = typename wrappers::shared<T>::template is_referable<T_>;

        template<class, class TProvider>
        static decltype(wrappers::shared<T>{std::shared_ptr<T>{std::shared_ptr<T>{std::declval<TProvider>().get()}}})
        try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider& provider) {
            return create_impl(provider);
        }

    private:
        template<class TProvider>
        auto create_impl(const TProvider& provider) {
            static std::shared_ptr<T> object{provider.get()};
            return wrappers::shared<T, std::shared_ptr<T>&>{object};
        }
    };
};

}}}} // boost::di::v1::scopes

#endif

