// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXPOSED_HPP
#define BOOST_DI_SCOPES_EXPOSED_HPP

#include <memory>
#include "boost/di/scopes/deduce.hpp"

namespace boost { namespace di { namespace scopes {

template<class TScope = scopes::deduce>
class exposed {
public:
    static constexpr auto priority = false;

    template<class TExpected, class TGiven>
    class scope {
        struct iprovider {
            virtual ~iprovider() = default;
            virtual std::unique_ptr<TExpected> get(const type_traits::unique&) const noexcept = 0;
            virtual std::shared_ptr<TExpected> get(const type_traits::shared&) const noexcept = 0;
            virtual TExpected get(const type_traits::stack&) const noexcept = 0;
        };

        template<typename TInjector>
        class provider : public iprovider {
        public:
            explicit provider(const TInjector& injector)
                : injector_(injector)
            { }

            std::unique_ptr<TExpected> get(const type_traits::unique&) const noexcept override {
                return injector_.template create<std::unique_ptr<TExpected>>();
            }

            std::shared_ptr<TExpected> get(const type_traits::shared&) const noexcept override {
                return injector_.template create<std::shared_ptr<TExpected>>();
            }

            TExpected get(const type_traits::stack&) const noexcept override {
                return injector_.template create<TExpected>();
            }

        private:
            TInjector injector_;
        };

        template<class TDeleter>
        struct provider_with_deleter {
            using deleter = TDeleter;

            template<class TMemory = type_traits::unique>
            auto get(const TMemory& memory = {}) const noexcept {
                return provider_.get(memory);
            }

            const iprovider& provider_;
        };

    public:
        template<class TInjector>
        explicit scope(const TInjector& injector) noexcept
            : provider_{std::make_shared<provider<TInjector>>(injector)}
        { }

        template<class T, class TProvider>
        auto create(const TProvider&) {
            using deleter = typename TProvider::deleter;
            return scope_.template create<T>(
                provider_with_deleter<deleter>{*provider_}
            );
        }

    private:
        std::shared_ptr<iprovider> provider_;
        typename TScope::template scope<TExpected, TGiven> scope_;
    };
};

}}} // boost::di::scopes

#endif

